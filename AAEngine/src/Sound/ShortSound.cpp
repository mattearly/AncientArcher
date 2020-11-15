#include <vector>
#include <AL\al.h>
#include <sndfile.h>
#include "../../include/Sound/ShortSound.h"
#include <unordered_map>
#include <iostream>
#include <AL\alext.h>
#include <inttypes.h>

namespace AA
{

// sound effect buffers so we know what to clean up and what not
static std::vector<ALuint> g_ShortSoundBuffers;

void ShortSound::Play(const ALuint buf)
{
	// update stored buffer to play if we need to
	if (buf != p_Buffer) {
		p_Buffer = buf;
		alSourcei(p_Source, AL_BUFFER, (ALint)buf);
	}

	alSourcePlay(p_Source);

	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "error in playing sound effect\n";
	}

	//state = AL_PLAYING;
	//std::cout << "playing sound\n";
	//while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	//{
	//	std::cout << "currently playing sound\n";
	//	alGetSourcei(source, AL_SOURCE_STATE, &state);
	//}
	//std::cout << "done playing sound\n";
}

void ShortSound::SetPosition(const float& x, const float& y, const float& z) {
	SetLocation(x, y, z);
}

void ShortSound::SetLocation(const float& x, const float& y, const float& z) {
	alSource3f(p_Source, AL_POSITION, x, y, z);
}

void ShortSound::SetPosition(const glm::vec3& loc)
{
	SetLocation(loc);
}

void ShortSound::SetLocation(const glm::vec3& loc)
{
	alSource3f(p_Source, AL_POSITION, loc.x, loc.y, loc.z);
}

void ShortSound::SetVolume(const float& gain)
{
	float newvol = 0;
	if (gain < 0.f)
	{
		newvol = 0;
	}
	else if (gain > 5.f)
	{
		newvol = 5.f;
	}
	else
	{
		newvol = gain;
	}

	alSourcef(p_Source, AL_GAIN, newvol);


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
	//	}
	//};
	//std::cout << geterrorstring(ALerror) << '\n';
}

ShortSound::ShortSound()
{

	float pitch = 1.f;
	float gain = 1.f;
	float position[3] = { 0,0,0 };
	float velocity[3] = { 0,0,0 };
	bool loop = false;

	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, pitch);
	alSourcef(p_Source, AL_GAIN, gain);
	alSource3f(p_Source, AL_POSITION, position[0], position[1], position[2]);
	alSource3f(p_Source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
	alSourcei(p_Source, AL_LOOPING, loop);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "error setting sound effect source data\n";
	}
}

ShortSound::~ShortSound()
{
	alDeleteSources(1, &p_Source);
}


ALuint ShortSound::AddShortSound(const char* filename)
{
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	/* Open the audio file and check that it's usable. */
	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
		sf_close(sndfile);
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */
	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	/* Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	/* Buffer the audio data into a new buffer object, then free the data and
	 * close the file.
	 */
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	g_ShortSoundBuffers.push_back(buffer);  // add to the list of known buffers

	return buffer;
}

bool ShortSound::RemoveShortSound(const ALuint& buf_to_remove)
{
	auto it = g_ShortSoundBuffers.begin();
	while (it != g_ShortSoundBuffers.end())
	{
		if (*it == buf_to_remove)
		{
			alDeleteBuffers(1, &*it);

			it = g_ShortSoundBuffers.erase(it);

			return true; //she was found and removed and our job is done here
		}
		else {
			++it;
		}
	}
	return false; //couldn't find, failed to remove
}

void ShortSound::DestroyAllBuffers()
{
	for (auto& b : g_ShortSoundBuffers)
	{
		alDeleteBuffers(1, &b);
	}

	g_ShortSoundBuffers.clear();

}

} //end namespace AA
