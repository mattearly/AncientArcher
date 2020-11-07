#include "../../include/Sound/SoundBufferManager.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>
#include <cassert>
#include "../../include/Loop.h"

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

uint32_t SoundBufferManager::addLongPlaySound(const char* filename)
{
	StreamBuffer *player;
	std::size_t frame_size;

	player->sndfile = sf_open(filename, SFM_READ, &player->sfinfo);
	if (!player->sndfile)
	{
		fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(NULL));
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */
	if (player->sfinfo.channels == 1)
		player->format = AL_FORMAT_MONO16;
	else if (player->sfinfo.channels == 2)
		player->format = AL_FORMAT_STEREO16;
	else if (player->sfinfo.channels == 3)
	{
		if (sf_command(player->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			player->format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (player->sfinfo.channels == 4)
	{
		if (sf_command(player->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			player->format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!player->format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", player->sfinfo.channels);
		sf_close(player->sndfile);
		player->sndfile = NULL;
		return 0;
	}

	frame_size = (size_t)(BUFFER_SAMPLES * player->sfinfo.channels) * sizeof(short);
	player->membuf = static_cast<short*>(malloc(frame_size));


	static uint32_t ourTracker = 0;
	ourTracker++;
	p_LongSounds.insert({ourTracker, player});
	return ourTracker;
}

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

int SoundBufferManager::PlayMusic(const uint32_t id, SoundSource& src)
{
	ALsizei i;

	/* Rewind the source position and clear the buffer queue */
	alSourceRewind(src.source);
	alSourcei(src.source, AL_BUFFER, 0);

	/* Fill the buffer queue */
	for (i = 0; i < NUM_MUSIC_BUFFERS; i++)
	{
		/* Get some data to give it to the buffer */
		//auto it = p_LongSounds.find(id);
		sf_count_t slen = sf_readf_short(p_LongSounds.at(id)->sndfile, p_LongSounds.at(id)->membuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= p_LongSounds.at(id)->sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(p_LongSounds.at(id)->buffers[i], p_LongSounds.at(id)->format, p_LongSounds.at(id)->membuf, (ALsizei)slen,
			p_LongSounds.at(id)->sfinfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(stderr, "Error buffering for playback\n");
		return 0;
	}

	/* Now queue and start playback! */
	alSourceQueueBuffers(src.source, i, p_LongSounds.at(id)->buffers);
	alSourcePlay(src.source);
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(stderr, "Error starting playback\n");
		return 0;
	}

	LOOP->addToSlowUpdate(SoundBufferManager::UpdatePlay(id, src));

	return 1;
}

int SoundBufferManager::UpdatePlay(const uint32_t id, SoundSource& src)
{
	ALint processed, state;

	/* Get relevant source info */
	alGetSourcei(src.source, AL_SOURCE_STATE, &state);
	alGetSourcei(src.source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		fprintf(stderr, "Error checking source state\n");
		return 0;
	}

	/* Unqueue and handle each processed buffer */
	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(src.source, 1, &bufid);
		processed--;

		/* Read the next chunk of data, refill the buffer, and queue it
		 * back on the source */
		slen = sf_readf_short(p_LongSounds.at(id)->sndfile, p_LongSounds.at(id)->membuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= p_LongSounds.at(id)->sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, p_LongSounds.at(id)->format, p_LongSounds.at(id)->membuf, (ALsizei)slen,
				p_LongSounds.at(id)->sfinfo.samplerate);
			alSourceQueueBuffers(src.source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			fprintf(stderr, "Error buffering data\n");
			return 0;
		}
	}

	/* Make sure the source hasn't underrun */
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		/* If no buffers are queued, playback is finished */
		alGetSourcei(src.source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
			return 0;

		alSourcePlay(src.source);
		if (alGetError() != AL_NO_ERROR)
		{
			fprintf(stderr, "Error restarting playback\n");
			return 0;
		}
	}

	return 1;
}
}  // end namespace AA