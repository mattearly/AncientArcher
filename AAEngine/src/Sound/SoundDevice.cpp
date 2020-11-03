#include "..\..\include\Sound\SoundDevice.h"
#include <AL\al.h>
namespace AA {

SoundDevice* SoundDevice::get()
{
	static SoundDevice* sound_device = new SoundDevice();
	return sound_device;
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr);  // get default device
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
		throw("failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))
		throw("failed to make context current");

	const ALCchar* name = NULL;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");

	alcDestroyContext(p_ALCContext);
	if (p_ALCContext)
		throw("failed to unset during close");

	if (!alcCloseDevice(p_ALCDevice))
		throw("failed to close sound device");
}

} // end namespace AA