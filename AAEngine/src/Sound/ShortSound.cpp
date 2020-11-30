#include <vector>
#include <AL\al.h>
#include <sndfile.h>
#include "../../include/Sound/ShortSound.h"
#include <unordered_map>
#include <AL\alext.h>
#include <inttypes.h>

/// <reference>
/// AL_PITCH f, fv pitch multiplier, always positive
/// AL_GAIN f, fv source gain value should be positive
/// AL_MAX_DISTANCE f, fv, i, iv used with the Inverse Clamped Distance Model
///                              to set the distance where there will no longer be
///                              any attenuation of the source
///                              AL_ROLLOFF_FACTOR f, fv, i, iv the rolloff rate for the source
///                              default is 1.0
/// AL_REFERENCE_DISTANCE f, fv, i, iv the distance under which the volume for the
///                                    source would normally drop by half(before
///                                    being influenced by rolloff factor or AL_MAX_DISTANCE)
/// AL_MIN_GAIN f, fv the minimum gain for this source
/// AL_MAX_GAIN f, fv the maximum gain for this source
/// AL_CONE_OUTER_GAIN f, fv the gain when outside the oriented cone
/// AL_CONE_INNER_ANGLE f, fv, i, iv the gain when inside the oriented cone
/// AL_CONE_OUTER_ANGLE f, fv, i, iv outer angle of the sound cone, in degrees
///                                  default is 360
/// AL_POSITION fv, 3f X, Y, Z position
/// AL_VELOCITY fv, 3f velocity vector
/// AL_DIRECTION fv, 3f, iv, 3i direction vector
/// AL_SOURCE_RELATIVE i, iv determines if the positions are relative to the listener
///                          default is AL_FALSE
/// AL_SOURCE_TYPE i, iv the source type – AL_UNDETERMINED, AL_STATIC, or AL_STREAMING
/// AL_LOOPING i, iv turns looping on(AL_TRUE) or off(AL_FALSE)
/// AL_BUFFER i, iv the ID of the attached buffer
/// AL_SOURCE_STATE i, iv the state of the source(AL_STOPPED, AL_PLAYING, …)
/// AL_BUFFERS_QUEUED* i, iv the number of buffers queued on this source
/// AL_BUFFERS_PROCESSED* i, iv the number of buffers in the queue that have been processed
/// AL_SEC_OFFSET f, fv, i, iv the playback position, expressed in seconds
/// AL_SAMPLE_OFFSET f, fv, i, iv the playback position, expressed in samples
/// AL_BYTE_OFFSET f, fv, i, iv the playback position, expressed in bytes
/// </reference>
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
		throw("error playing short sound");
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

	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound location");

	}
}

void ShortSound::SetPosition(const glm::vec3& loc)
{
	SetLocation(loc);
}

void ShortSound::SetLocation(const glm::vec3& loc)
{
	alSource3f(p_Source, AL_POSITION, loc.x, loc.y, loc.z);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound Location");
	}
}

void ShortSound::SetVelocity(float velocity)
{
	alSourcef(p_Source, AL_VELOCITY, velocity);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound velocity");
	}
}

void ShortSound::SetDirection(const float& x, const float& y, const float& z)
{
	alSource3f(p_Source, AL_DIRECTION, x, y, z);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound direction");
	}
}

void ShortSound::SetDirection(const glm::vec3& dir)
{
	SetDirection(dir.x, dir.y, dir.z);
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
		throw("error setting short sound volume");
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

void ShortSound::SetLooping(const bool& opt)
{
	alSourcei(p_Source, AL_LOOPING, (ALint)opt);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound looping status");
	}
}
/// <summary>
/// determines if the positions are relative to the listener
/// </summary>
/// <param name="opt"></param>
void ShortSound::SetRelative(const bool& opt)
{
	alSourcei(p_Source, AL_SOURCE_RELATIVE, (ALint)opt);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound relative");
	}
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
	alSourcei(p_Source, AL_LOOPING, (ALint)loop);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting short sound data");
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
#ifdef _DEBUG
		fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
#endif
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
#ifdef _DEBUG
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
#endif
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
#ifdef _DEBUG
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
#endif
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
#ifdef _DEBUG
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
#endif
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
#ifdef _DEBUG
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
#endif
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
