#include "../../include/Sound/SoundDevice.h"
#include "../../include/Sound/SoundListener.h"

namespace AA {
SoundListener* SoundListener::Get()
{
	SoundListener* snd_listener = new SoundListener();
	return snd_listener;
}

void SoundListener::SetMasterGain(const float& gain)
{
	// consider gain clamps
	float newVol = gain;
	if (newVol < 0.f)
	{
		newVol = 0.f;
	}
	else if (newVol > 30.f)
	{
		// now thats flippin loud, lets cap it
		newVol = 30.f;
	}

	alListenerf(AL_GAIN, newVol);

	if (alGetError() != AL_NO_ERROR)
	{
		throw("error setting gain");
	}
}

void SoundListener::SetPosition(const glm::vec3& pos)
{
	SetLocation(pos);
}

void SoundListener::SetLocation(const glm::vec3& pos)
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void SoundListener::SetPosition(const float& x, const float& y, const float& z)
{
	SetLocation(x, y, z);
}

void SoundListener::SetLocation(const float& x, const float& y, const float& z)
{
	alListener3f(AL_POSITION, x, y, z);

}

void SoundListener::SetOrientation(const glm::vec3& at, const glm::vec3& up)
{
}

void SoundListener::SetOrientation(const float& xat, const float& yat, const float& zat, const float& xup, const float& yup, const float& zup)
{
}

SoundListener::~SoundListener()
{
}


}