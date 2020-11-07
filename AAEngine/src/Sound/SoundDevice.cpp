#include "..\..\include\Sound\SoundDevice.h"
#include <AL\al.h>
namespace AA {

static SoundDevice* p_SoundDevice_instance = nullptr;

SoundDevice* SoundDevice::get()
{
	init(); // uses default device unless device was custom init
	return p_SoundDevice_instance;
}

void SoundDevice::init()
{
	if (!p_SoundDevice_instance)
	{
		p_SoundDevice_instance = new SoundDevice();
	}
}

void SoundDevice::init(const char* devicename)
{
	if (!p_SoundDevice_instance)
	{
		p_SoundDevice_instance = new SoundDevice(devicename);
	}
}

void SoundDevice::SwitchDevice(const char* devicename)
{
	//close previous device
	if (!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");

	alcDestroyContext(p_ALCContext);
	if (p_ALCContext)
		throw("failed to unset during close");

	if (!alcCloseDevice(p_ALCDevice))
		throw("failed to close sound device");

	//open new device
	p_ALCDevice = alcOpenDevice(devicename);
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
		throw("failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))
		throw("failed to make context current");


	//verbose
	const ALCchar* name = NULL;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

void SoundDevice::populateDeviceVec(std::vector<std::string>& devicesVec)
{
	const ALCchar* devicenames;
	//devicenames = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER); // get current without windows tag
	devicenames = alcGetString(nullptr, ALC_DEVICE_SPECIFIER); // get all without windows tag
	////devicenames = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER); // get all with windows tag
	//devicenames = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER); // get current with windows tag

	const char* ptr = devicenames;

	devicesVec.clear();

	do
	{
		devicesVec.push_back(std::string(ptr));
		ptr += devicesVec.back().size() + 1;
	} while (*(ptr + 1) != '\0');

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


	//verbose say device name opened
	const ALCchar* name = NULL;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::SoundDevice(const char* devicename)
{
	p_ALCDevice = alcOpenDevice(devicename);
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
		throw("failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))
		throw("failed to make context current");


	//verbose say device name opened
	const ALCchar* name = NULL;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}


SoundDevice::~SoundDevice()
{
	//if (!alcMakeContextCurrent(nullptr))
	//	throw("failed to set context to nullptr");

	//alcDestroyContext(p_ALCContext);
	//if (p_ALCContext)
	//	throw("failed to unset during close");

	//if (!alcCloseDevice(p_ALCDevice))
	//	throw("failed to close sound device");

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}

} // end namespace AA