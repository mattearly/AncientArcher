#include "../../include/Sound/LongSound.h"
#include "../../include/Sound/SoundDevice.h"
#include <cassert>
#include <iostream>
#include <AL\alc.h>
#include <AL\alext.h>

namespace AA {

void LongSound::Play()
{
	// if already playing, don't do anything
	if (GetPlayingState() == AL_PLAYING)
		return;

	ALsizei i;

	// clear any al errors
	if (alGetError() != AL_NO_ERROR) {
		throw("pre-existing sound error when attempting playback of long sound");
	}

	/* Rewind the source position and clear the buffer queue */
	alSourceRewind(p_Source);  //?
	alSourcei(p_Source, AL_BUFFER, 0); //?

	/* Fill the buffer queue */
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		/* Get some data to give it to the buffer */
		sf_count_t slen = sf_readf_short(p_Sndfile, p_Membuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error buffering for playback");
	}

	/* Now queue and start playback! */
	alSourceQueueBuffers(p_Source, i, p_Buffers);
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error starting playback");
	}

}

void LongSound::Pause()
{
	// if not currently Playing, pause does nothing
	if (GetPlayingState() != AL_PLAYING)
		return;
	
	alSourcePause(p_Source);
}

void LongSound::Resume()
{
	// do nothing if not paused
	if (GetPlayingState() != AL_PAUSED)
		return;

	alSourcePlay(p_Source);
}

void LongSound::Stop() {
	// if already stopped, do nothing
	if (GetPlayingState() == AL_STOPPED)
		return;

	alSourceStop(p_Source);
}

void LongSound::SetVolume(const float& val)
{
	float newvolume = val;
	if (newvolume < 0.f)  //can't be negative
	{
		newvolume = 0.f;
	}
	else if (newvolume > 5.f)
	{
		// cap at 5
		newvolume = 5.f;
	}

	//SoundDevice::Get()->SuspendContext();
	alSourcef(p_Source, AL_GAIN, newvolume);
	//SoundDevice::Get()->ResumeContext();


	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting gain");
	}

	//ALenum ALerror = AL_NO_ERROR;
	//ALerror = alGetError();
	//auto geterrorstring = [](ALenum ALerror) {
	//	switch (ALerror) {
	//	case AL_NO_ERROR:       return std::string("AL_NO_ERROR - (No error)."); break;
	//	case AL_INVALID_NAME:       return std::string("AL_INVALID_NAME - Invalid Name paramater passed to AL call."); break;
	//	case AL_INVALID_ENUM:       return std::string("AL_INVALID_ENUM - Invalid parameter passed to AL call."); break;
	//	case AL_INVALID_VALUE:      return std::string("AL_INVALID_VALUE - Invalid enum parameter value."); break;
	//	case AL_INVALID_OPERATION:  return std::string("AL_INVALID_OPERATION"); break;
	//	case AL_OUT_OF_MEMORY:      return std::string("AL_OUT_OF_MEMORY"); break;
	//	default:            return std::string("AL Unknown Error."); break;
	//	}; };
	//std::cout << geterrorstring(ALerror) << '\n';
}

void LongSound::UpdatePlayBuffer()
{
	if (GetPlayingState() != AL_PLAYING)
		return;



	ALint processed, state;

	// clear error 
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "Error before rebuffering\n";
	}

	/* Get relevant source info */
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(stderr, "Error checking source state\n");
		return;
	}

	/* Unqueue and handle each processed buffer */
	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(p_Source, 1, &bufid);
		processed--;

		/* Read the next chunk of data, refill the buffer, and queue it
		 * back on the source */
		slen = sf_readf_short(p_Sndfile, p_Membuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
			alSourceQueueBuffers(p_Source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			fprintf(stderr, "Error buffering data\n");
			return;
		}
	}

	/* Make sure the source hasn't underrun */
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		/* If no buffers are queued, playback is finished */
		alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
			return;

		alSourcePlay(p_Source);
		if (alGetError() != AL_NO_ERROR)
		{
			fprintf(stderr, "Error restarting playback\n");
			return;
		}
	}

}

ALint LongSound::GetPlayingState()
{
	ALint curr_state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &curr_state);
	return curr_state;
}

//LongSound::LongSound()
//{
//	alGenSources(1, &p_Source);
//	assert(alGetError() == AL_NO_ERROR && "Could not create source");
//
//	/* Set parameters so mono sources play out the front-center speaker and
//		* won't distance attenuate. */
//		//alSource3i(p_Source, AL_POSITION, 0, 0, -1);
//	//alSourcei(p_Source, AL_SOURCE_RELATIVE, AL_TRUE);
//	//alSourcei(p_Source, AL_ROLLOFF_FACTOR, 0);
//	//assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");
//}

LongSound::LongSound(const char* filename)
{
	alGenSources(1, &p_Source);

	alGenBuffers(BUFFER_COUNT, p_Buffers);
	std::size_t frame_size;

	p_Sndfile = sf_open(filename, SFM_READ, &p_Sfinfo);
	if (!p_Sndfile)
	{
		//fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(NULL));
		throw("Could not open audio file");
	}

	/* Get the sound format, and figure out the OpenAL format */
	if (p_Sfinfo.channels == 1)
		p_Format = AL_FORMAT_MONO16;
	else if (p_Sfinfo.channels == 2)
		p_Format = AL_FORMAT_STEREO16;
	else if (p_Sfinfo.channels == 3)
	{
		if (sf_command(p_Sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (p_Sfinfo.channels == 4)
	{
		if (sf_command(p_Sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT3D_16;
	}

	if (!p_Format)
	{
		//fprintf(stderr, "Unsupported channel count: %d\n", p_Sfinfo.channels);
		sf_close(p_Sndfile);
		p_Sndfile = NULL;
		throw("Unsupported channel count in opening new sound file");
	}

	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
	p_Membuf = static_cast<short*>(malloc(frame_size));

}

LongSound::~LongSound()
{
	//routine: delete all buffer data and snd file stuff
	//close player file
	if (p_Sndfile)
	{
		sf_close(p_Sndfile);
	}
	p_Sndfile = nullptr;
	free(p_Membuf);
	p_Membuf = nullptr;

	//delete sources
	alDeleteSources(1, &p_Source);

	//delete buffers
	alDeleteBuffers(BUFFER_COUNT, p_Buffers);
}

};