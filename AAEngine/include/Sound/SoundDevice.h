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

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

};


}  //end namespace AA