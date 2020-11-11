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

	/// <summary>
	/// Initializes a new buffer for playing this particular long sound. 
	/// In an ideal world, you should remove it before adding or using another one.
	/// </summary>
	/// <param name="filename">A filepath to a music or long sound file.</param>
	/// <returns>sound play id</returns>
	uint32_t SoundBufferManager::addLongPlaySound(const char* filename)
	{

		MusicStream* music_stream;
		music_stream = static_cast<MusicStream*>(calloc(1, sizeof(*music_stream)));
		assert(music_stream != nullptr);

		alGenBuffers(NUM_MUSIC_BUFFERS, music_stream->buffers);

		std::size_t frame_size;

		music_stream->sndfile = sf_open(filename, SFM_READ, &music_stream->sfinfo);
		if (!music_stream->sndfile)
		{
			fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(NULL));
			return 0;
		}

		/* Get the sound format, and figure out the OpenAL format */
		if (music_stream->sfinfo.channels == 1)
			music_stream->format = AL_FORMAT_MONO16;
		else if (music_stream->sfinfo.channels == 2)
			music_stream->format = AL_FORMAT_STEREO16;
		else if (music_stream->sfinfo.channels == 3)
		{
			if (sf_command(music_stream->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				music_stream->format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (music_stream->sfinfo.channels == 4)
		{
			if (sf_command(music_stream->sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				music_stream->format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!music_stream->format)
		{
			fprintf(stderr, "Unsupported channel count: %d\n", music_stream->sfinfo.channels);
			sf_close(music_stream->sndfile);
			music_stream->sndfile = NULL;
			return 0;
		}

		frame_size = ((size_t)BUFFER_SAMPLES * (size_t)music_stream->sfinfo.channels) * sizeof(short);
		music_stream->membuf = static_cast<short*>(malloc(frame_size));


		static uint32_t ourTrackerId = 0;
		ourTrackerId++;
		p_LongSounds.insert({ ourTrackerId, music_stream });

		return ourTrackerId;
	}

	SoundBufferManager::SoundBufferManager()
	{
		p_SoundEffectBuffers.clear();
		p_LongSounds.clear();
	}

	SoundBufferManager::~SoundBufferManager()
	{
		alDeleteBuffers((ALsizei)p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());  // remove all with 1 call
		p_SoundEffectBuffers.clear(); //probably not necessary but whatevs

		for (auto& ms : p_LongSounds)
		{
			// close open sound file stream
			if (ms.second->sndfile)
				sf_close(ms.second->sndfile);
			ms.second->sndfile = nullptr;

			free(ms.second->membuf);
			ms.second->membuf = nullptr;
			// delete buffers for music stream
			alDeleteBuffers(NUM_MUSIC_BUFFERS, ms.second->buffers);
			if (alGetError() != AL_NO_ERROR)
				fprintf(stderr, "failed to delete obj ids\n");

			memset(ms.second, 0, sizeof(*ms.second));
			free(ms.second);
		}

		p_LongSounds.clear();

	}

	int SoundBufferManager::StartLongSoundPlay(const uint32_t id, const ALuint src)
	{
		ALsizei i;

		// clear any al errors
		alGetError();
		/* Rewind the source position and clear the buffer queue */
		alSourceRewind(src);
		alSourcei(src, AL_BUFFER, 0);

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
		alSourceQueueBuffers(src, i, p_LongSounds.at(id)->buffers);
		alSourcePlay(src);
		if (alGetError() != AL_NO_ERROR)
		{
			fprintf(stderr, "Error starting playback\n");
			return 0;
		}

		//LOOP->addToSlowUpdate(SoundBufferManager::UpdatePlay(id, *src));

		return 1;
	}

	int SoundBufferManager::UpdateLongPlayingBuffers(const uint32_t id, const ALuint src)
	{
		ALint processed, state;

		// clear error 
		alGetError();
		/* Get relevant source info */
		alGetSourcei(src, AL_SOURCE_STATE, &state);
		alGetSourcei(src, AL_BUFFERS_PROCESSED, &processed);
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

			alSourceUnqueueBuffers(src, 1, &bufid);
			processed--;

			/* Read the next chunk of data, refill the buffer, and queue it
			 * back on the source */
			slen = sf_readf_short(p_LongSounds.at(id)->sndfile, p_LongSounds.at(id)->membuf, BUFFER_SAMPLES);
			if (slen > 0)
			{
				slen *= p_LongSounds.at(id)->sfinfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, p_LongSounds.at(id)->format, p_LongSounds.at(id)->membuf, (ALsizei)slen,
					p_LongSounds.at(id)->sfinfo.samplerate);
				alSourceQueueBuffers(src, 1, &bufid);
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
			alGetSourcei(src, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0)
				return 0;

			alSourcePlay(src);
			if (alGetError() != AL_NO_ERROR)
			{
				fprintf(stderr, "Error restarting playback\n");
				return 0;
			}
		}

		return 1;
	}
}  // end namespace AA