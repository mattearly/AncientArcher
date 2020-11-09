#include "../AAEngine/include/Loop.h"
#include "../AAEngine/include/Sound/SoundDevice.h"
#include "../AAEngine/include/Sound/SoundBufferManager.h"
#include "../AAEngine/include/Sound/SoundEffectSource.h"
#include "../AAEngine/include/Sound/SoundMusicSource.h"
#include <iostream>

using namespace AA;

int main()
{
	SoundDevice* sound_dev = SoundDevice::get(); // should init sound device and not throw if successful
	std::vector<std::string> devices;
	sound_dev->populateDeviceVec(devices);
	std::cout << "List of Devices: \n";
	for (const auto& d : devices)
	{
		std::cout << '-' << d << std::endl;
	}

	static uint32_t heal_sound_buf = SoundBufferManager::get()->addSoundEffect("../assets/sounds/heal.ogg");
	static SoundEffectSource heal_sound;
	
	static uint32_t kwon_song = SoundBufferManager::get()->addLongPlaySound("../assets/sounds/music/Into It - Kwon.ogg");
	static SoundMusicSource music_sounds;

	static bool isPlaying = false;
	//static bool isPaused = false;
	auto ourControls = [](KeyboardInput& in) {
		if (in.mouseButton1)
		{
			heal_sound.Play(heal_sound_buf);
		}
		if (in.p)
		{
		
			if(!isPlaying)
			{
				music_sounds.Play(kwon_song);
				isPlaying = true;
			}
			/*else if (!isPaused) 
			{
				music_sounds.Pause(kwon_song);
				isPaused = true;
			}
			else if (isPaused)
			{
				music_sounds.Play(kwon_song);
				isPaused = false;
			}*/
		}
	};
	LOOP->addToKeyHandling(ourControls);

	auto updateMusicStream = []()
	{
		if (isPlaying)
		{
			music_sounds.UpdatePlayingBuffers(kwon_song);
		}
	};
	LOOP->addToSlowUpdate(updateMusicStream);

	return LOOP->runMainLoop();
}