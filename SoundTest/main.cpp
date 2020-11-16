#include "../AAEngine/include/Loop.h"
#include "../AAEngine/include/Sound/SoundDevice.h"
#include "../AAEngine/include/Sound/ShortSound.h"
#include "../AAEngine/include/Sound/LongSound.h"
#include "../AAEngine/include/Sound/SoundListener.h"
#include <iostream>

using namespace AA;

int main()
{
	SoundDevice* sound_dev = SoundDevice::Get(); // should init sound device and not throw if successful
	std::vector<std::string> devices;
	sound_dev->PopulateDeviceVec(devices);
	std::cout << "List of Devices: \n";
	for (const auto& d : devices)
	{
		std::cout << '-' << d << std::endl;
	}

	SoundListener::Get()->SetPosition(0.f, 0.f, 0.f);
	SoundListener::Get()->SetOrientation(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));


	static uint32_t heal_sound_buf = ShortSound::AddShortSound("../assets/sounds/heal.ogg");
	static ShortSound heal_sound;
	heal_sound.SetPosition(glm::vec3(-2.f, 0.f, 0.f));
	//heal_sound.SetRelative(true);

	static LongSound music_sounds("../assets/sounds/music/Into It - Kwon.ogg");

	static float master_volume = 1.f;
	static float music_volume = 1.f;
	static float se_volume = 1.f;
	auto playControls = [](KeyboardInput& in)
	{
		if (in.mouseButton1)  // heal sound
		{
			heal_sound.Play(heal_sound_buf);
			return true;
		}

		if (in.p)  //play | pause toggle
		{
			music_sounds.Play();
			return true;
		}

		if (in.semiColon)
		{
			music_sounds.Pause();
			return true;
		}

		if (in.l)
		{
			music_sounds.Resume();
			return true;
		}

		if (in.o)  // stop
		{
			music_sounds.Stop();
			return true;
		}
		return false;
	};
	LOOP->addToTimedOutKeyHandling(playControls);

	auto timedKeybinds = [](KeyboardInput& in)
	{
		if (in.upArrow)
		{
			master_volume += .1f;
			if (master_volume > 3.f)
			{
				master_volume = 3.f;
			}
			float passvol = master_volume;
			std::cout << "Master Volume: " << master_volume << '\n';
			SoundListener::Get()->SetMasterGain(passvol);

			return true;
		}

		if (in.downArrow)
		{
			master_volume -= .1f;
			if (master_volume < 0.f)
			{
				master_volume = 0.f;
			}
			std::cout << "Master Volume: " << master_volume << '\n';
			float passvol = master_volume;
			SoundListener::Get()->SetMasterGain(passvol);
			return true;
		}

		if (in.equal)
		{
			music_volume += .1f;
			if (music_volume > 3.f)
			{
				music_volume = 3.f;
			}
			float newvol = music_volume;
			std::cout << "Music Volume: " << music_volume << '\n';
			music_sounds.SetVolume(newvol);
			return true;
		}

		if (in.minus)
		{
			music_volume -= .1f;
			if (music_volume < 0.0001f)
			{
				music_volume = 0.0001f;
			}
			float newvol = music_volume;
			std::cout << "Music Volume: " << music_volume << '\n';
			music_sounds.SetVolume(newvol);
			return true;
		}

		if (in.period)
		{
			se_volume += .1f;
			if (se_volume > 3.f)
			{
				se_volume = 3.f;
			}
			float newvol = se_volume;
			std::cout << "SE Volume: " << se_volume << '\n';
			heal_sound.SetVolume(newvol);
			return true;
		}

		if (in.comma)
		{
			se_volume -= .1f;
			if (se_volume < 0.f)
			{
				se_volume = 0.f;
			}
			float newvol = se_volume;
			std::cout << "SE Volume: " << se_volume << '\n';
			heal_sound.SetVolume(newvol);
			return true;
		}

		return false;
	};
	LOOP->addToTimedOutKeyHandling(timedKeybinds);

	auto updateMusicBuffer = []() {
		music_sounds.UpdatePlayBuffer();  //todo: move this internally into LongSound
	};
	LOOP->addToSlowUpdate(updateMusicBuffer);

	//LOOP->setSlowUpdateTimeoutLength(.05f);

	auto destroySounds = []()
	{
		ShortSound::DestroyAllBuffers();
	};
	LOOP->addToOnTeardown(destroySounds);

	return LOOP->runMainLoop();

}