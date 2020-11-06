#include "../../include/Sound/SoundBufferManager.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>
#include <cassert>

namespace AA {
SoundBufferManager* SoundBufferManager::get()
{
	static SoundBufferManager* sndbuf = new SoundBufferManager();
	return sndbuf;
}

ALuint SoundBufferManager::addSoundEffect(const char* filename)
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

	p_SoundEffectBuffers.push_back(buffer);  // add to the list of known buffers

	return buffer;
}

bool SoundBufferManager::removeSoundEffect(const ALuint& buf_to_remove)
{
	auto it = p_SoundEffectBuffers.begin();
	while (it != p_SoundEffectBuffers.end())
	{
		if (*it == buf_to_remove)
		{
			alDeleteBuffers(1, &*it);

			it = p_SoundEffectBuffers.erase(it);

			return true; //she was found and removed and our job is done here
		}
		else {
			++it;
		}
	}
	return false; //couldn't find, failed to remove
}

//void SoundBufferManager::InitPlayer(SoundSource& source)
//{
//	StreamPlayer* player;
//
//	player = static_cast<AA::StreamPlayer*>(calloc(1, sizeof(*player)));
//	assert(player != NULL);
//
//	/* Generate the buffers and source */
//	alGenBuffers(NUM_MUSIC_BUFFERS, player->buffers);
//	assert(alGetError() == AL_NO_ERROR && "Could not create buffers");
//
//	//alGenSources(1, &player->source);
//	alGenSources(1, &source.source);
//	assert(alGetError() == AL_NO_ERROR && "Could not create source");
//
//	/* Set parameters so mono sources play out the front-center speaker and
//	 * won't distance attenuate. */
//	alSource3i(source.source, AL_POSITION, 0, 0, -1);
//	alSourcei(source.source, AL_SOURCE_RELATIVE, AL_TRUE);
//	alSourcei(source.source, AL_ROLLOFF_FACTOR, 0);
//	assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");
//
//	p_StreamPlayer = player;
//}
//
//void SoundBufferManager::DeletePlayer()
//{
//	ClosePlayerFile();
//	alDeleteBuffers(NUM_MUSIC_BUFFERS, p_StreamPlayer->buffers);
//	if (alGetError() != AL_NO_ERROR)
//		throw("error delete music player");
//
//	memset(p_StreamPlayer, 0, sizeof(*p_StreamPlayer));
//	free(p_StreamPlayer);
//}
//
//int SoundBufferManager::OpenPlayerFile(const char* filename)
//{
//	std::size_t frame_size;
//
//	ClosePlayerFile();
//
//	/* Open the audio file and check that it's usable. */
//	p_StreamPlayer->sndfile = sf_open(filename, SFM_READ, &p_StreamPlayer->sfinfo);
//	if (!p_StreamPlayer->sndfile)
//	{
//		fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(NULL));
//		return 0;
//	}
//
//	/* Get the sound format, and figure out the OpenAL format */
//	if (p_StreamPlayer->sfinfo.channels == 1)
//		p_StreamPlayer->format = AL_FORMAT_MONO16;
//	else if (p_StreamPlayer->sfinfo.channels == 2)
//		p_StreamPlayer->format = AL_FORMAT_STEREO16;
//	else if (p_StreamPlayer->sfinfo.channels == 3)
//	{
//		if (sf_command(p_StreamPlayer->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//			p_StreamPlayer->format = AL_FORMAT_BFORMAT2D_16;
//	}
//	else if (p_StreamPlayer->sfinfo.channels == 4)
//	{
//		if (sf_command(p_StreamPlayer->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
//			p_StreamPlayer->format = AL_FORMAT_BFORMAT3D_16;
//	}
//	if (!p_StreamPlayer->format)
//	{
//		fprintf(stderr, "Unsupported channel count: %d\n", p_StreamPlayer->sfinfo.channels);
//		sf_close(p_StreamPlayer->sndfile);
//		p_StreamPlayer->sndfile = NULL;
//		return 0;
//	}
//
//	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_StreamPlayer->sfinfo.channels) * sizeof(short);
//	p_StreamPlayer->membuf = static_cast<short*>(malloc(frame_size));
//
//	return 1;
//}
//
//void SoundBufferManager::ClosePlayerFile()
//{
//	if (p_StreamPlayer) {
//		if (p_StreamPlayer->sndfile)
//			sf_close(p_StreamPlayer->sndfile);
//
//		free(p_StreamPlayer->membuf);
//		p_StreamPlayer->membuf = nullptr;
//	}
//}
//
//int SoundBufferManager::StartPlayer(SoundSource& source)
//{
//	ALsizei i;
//
//	/* Rewind the source position and clear the buffer queue */
//	alSourceRewind(source.source);
//	alSourcei(source.source, AL_BUFFER, 0);
//
//	/* Fill the buffer queue */
//	for (i = 0; i < NUM_MUSIC_BUFFERS; i++)
//	{
//		/* Get some data to give it to the buffer */
//		sf_count_t slen = sf_readf_short(p_StreamPlayer->sndfile, p_StreamPlayer->membuf, BUFFER_SAMPLES);
//		if (slen < 1) break;
//
//		slen *= p_StreamPlayer->sfinfo.channels * (sf_count_t)sizeof(short);
//		alBufferData(p_StreamPlayer->buffers[i], p_StreamPlayer->format, p_StreamPlayer->membuf, (ALsizei)slen, p_StreamPlayer->sfinfo.samplerate);
//	}
//	if (alGetError() != AL_NO_ERROR)
//	{
//		fprintf(stderr, "Error buffering for playback\n");
//		return 0;
//	}
//
//	/* Now queue and start playback! */
//	alSourceQueueBuffers(source.source, i, p_StreamPlayer->buffers);
//	alSourcePlay(source.source);
//	if (alGetError() != AL_NO_ERROR)
//	{
//		fprintf(stderr, "Error starting playback\n");
//		return 0;
//	}
//
//	return 1;
//}
//
//int SoundBufferManager::UpdatePlayer(SoundSource &source)
//{
//	ALint processed, state;
//
//	/* Get relevant source info */
//	alGetSourcei(source.source, AL_SOURCE_STATE, &state);
//	alGetSourcei(source.source, AL_BUFFERS_PROCESSED, &processed);
//	if (alGetError() != AL_NO_ERROR)
//	{
//		fprintf(stderr, "Error checking source state\n");
//		return 0;
//	}
//
//	/* Unqueue and handle each processed buffer */
//	while (processed > 0)
//	{
//		ALuint bufid;
//		sf_count_t slen;
//
//		alSourceUnqueueBuffers(source.source, 1, &bufid);
//		processed--;
//
//		/* Read the next chunk of data, refill the buffer, and queue it
//		 * back on the source */
//		slen = sf_readf_short(p_StreamPlayer->sndfile, p_StreamPlayer->membuf, BUFFER_SAMPLES);
//		if (slen > 0)
//		{
//			slen *= p_StreamPlayer->sfinfo.channels * (sf_count_t)sizeof(short);
//			alBufferData(bufid, p_StreamPlayer->format, p_StreamPlayer->membuf, (ALsizei)slen,
//				p_StreamPlayer->sfinfo.samplerate);
//			alSourceQueueBuffers(source.source, 1, &bufid);
//		}
//		if (alGetError() != AL_NO_ERROR)
//		{
//			fprintf(stderr, "Error buffering data\n");
//			return 0;
//		}
//	}
//
//	/* Make sure the source hasn't underrun */
//	if (state != AL_PLAYING && state != AL_PAUSED)
//	{
//		ALint queued;
//
//		/* If no buffers are queued, playback is finished */
//		alGetSourcei(source.source, AL_BUFFERS_QUEUED, &queued);
//		if (queued == 0)
//			return 0;
//
//		alSourcePlay(source.source);
//		if (alGetError() != AL_NO_ERROR)
//		{
//			fprintf(stderr, "Error restarting playback\n");
//			return 0;
//		}
//	}
//
//	return 1;
//}

SoundBufferManager::SoundBufferManager()
{
	p_SoundEffectBuffers.clear();
	//p_StreamPlayer = nullptr;
	//p_MusicBuffers[0] = 0;
	//p_MusicBuffers[1] = 0;
	//p_MusicBuffers[2] = 0;
	//p_MusicBuffers[3] = 0;
}

SoundBufferManager::~SoundBufferManager()
{
	alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());  // remove all with 1 call
	p_SoundEffectBuffers.clear(); //probably not necessary but whatevs

	//alDeleteBuffers(NUM_MUSIC_BUFFERS, &p_MusicBuffers[0]);
	//p_MusicBuffers[0] = 0;
	//p_MusicBuffers[1] = 0;
	//p_MusicBuffers[2] = 0;
	//p_MusicBuffers[3] = 0;
}
}  // end namespace AA