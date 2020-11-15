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

	static uint32_t heal_sound_buf = ShortSound::AddShortSound("../assets/sounds/heal.ogg");
	static ShortSound heal_sound;

	//static uint32_t kwon_song = SoundBufferManager::get()->addLongPlaySound("../assets/sounds/music/Into It - Kwon.ogg");
	static LongSound music_sounds("../assets/sounds/music/Into It - Kwon.ogg");

	static enum class MUSIC_STATE {
		initial, play, pause, stop
	} music_state = MUSIC_STATE::initial;

	static float master_volume = 1.f;
	static float music_volume = 1.f;
	static float se_volume = 1.f;
	auto play_stop_controls = [](KeyboardInput& in)
	{
		if (in.mouseButton1)  // heal sound
		{
			heal_sound.Play(heal_sound_buf);
			return true;
		}
		if (in.p)  //play | pause toggle
		{
			if (music_state != MUSIC_STATE::play)
			{
				music_sounds.Play();
				music_state = MUSIC_STATE::play;
				return true;
			}
			else if (music_state == MUSIC_STATE::play)
			{
				music_sounds.Pause();
				music_state = MUSIC_STATE::pause;
				return true;
			}
		}
		if (in.o)  // stop if playing or pause
		{
			if (music_state == MUSIC_STATE::play || music_state == MUSIC_STATE::pause)
			{
				//music_sounds.Stop();
				music_state = MUSIC_STATE::stop;
				return true;
			}
		}
		return false;
	};
	LOOP->addToTimedOutKeyHandling(play_stop_controls);

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
			if (music_volume < 0.f)
			{
				music_volume = 0.f;
			}
			float newvol = music_volume;
			std::cout << "Music Volume: " << music_volume << '\n';
			heal_sound.SetVolume(newvol);
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
		if (music_state == MUSIC_STATE::play)
		{
			music_sounds.UpdatePlayBuffer();
		}
	};
	LOOP->addToSlowUpdate(updateMusicBuffer);

	return LOOP->runMainLoop();
}