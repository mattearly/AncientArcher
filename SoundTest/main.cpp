#include "../AAEngine/include/Loop.h"
#include "../AAEngine/include/Sound/SoundDevice.h"
#include "../AAEngine/include/Sound/SoundBuffers.h"
#include "../AAEngine/include/Sound/SoundSource.h"
#include <iostream>

using namespace AA;

int main()
{
	SoundDevice* sound_dev = SoundDevice::get(); // should init sound device and not throw if successful
	static uint32_t heal_sound_buf = SoundBuffers::get()->addSound("../assets/sounds/heal.ogg");
	static SoundSource heal_sound;
	auto playDingOnClick = [](KeyboardInput& in) {
		if (in.mouseButton1)
		{
			heal_sound.Play(heal_sound_buf);
		}
	};
	LOOP->addToKeyHandling(playDingOnClick);

	return LOOP->runMainLoop();
}