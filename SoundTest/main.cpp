#include "../AAEngine/include/Loop.h"
#include "../AAEngine/include/Sound/SoundDevice.h"
#include "../AAEngine/include/Sound/SoundBuffers.h"
#include "../AAEngine/include/Sound/SoundSource.h"
#include <iostream>

using namespace AA;

int main()
{
	SoundDevice* sound_dev = SoundDevice::get(); // should init sound device and not throw if successful
	static uint32_t ding = SoundBuffers::get()->addSound("../assets/sounds/heal.ogg");
	static SoundSource cowbellding;
	auto playDingOnClick = [](KeyboardInput& in) {
		if (in.mouseButton1)
		{
			std::cout << "play ding\n";

			cowbellding.Play(ding);
		}
	};
	LOOP->addToKeyHandling(playDingOnClick);

	return LOOP->runMainLoop();
}