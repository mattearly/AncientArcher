#pragma once
#include <vector>
#include <string>
#include <AL\alc.h>

namespace AA {

/// <summary>
/// By calling get() the first time, SoundDevice sets up the sound device and context for the device.
/// </summary>
class SoundDevice
{
public:
	static SoundDevice* get();
	static void init();
	static void init(const char* devicename);

	void SwitchDevice(const char* devicename);

	void populateDeviceVec(std::vector<std::string>& devicesVec);

protected:
	SoundDevice();
	SoundDevice(const char* devicename);
	~SoundDevice();

private:

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

};


}  //end namespace AA