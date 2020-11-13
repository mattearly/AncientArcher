#include "../AAEngine/include/Loop.h"
#include "../AAEngine/include/Sound/SoundDevice.h"
#include "../AAEngine/include/Sound/SoundBufferManager.h"
#include "../AAEngine/include/Sound/SoundEffectSource.h"
#include "../AAEngine/include/Sound/SoundMusicSource.h"
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

	static uint32_t heal_sound_buf = SoundBufferManager::get()->addSoundEffect("../assets/sounds/heal.ogg");
	static SoundEffectSource heal_sound;
	
	static uint32_t kwon_song = SoundBufferManager::get()->addLongPlaySound("../assets/sounds/music/Into It - Kwon.ogg");
	static SoundMusicSource music_sounds;

	static enum class MUSIC_STATE {
		play, pause, stop
	} music_state;
	static float master_volume = 1.f;
	static float music_volume = 1.f;
	static float se_volume = 1.f;
	auto play_stop_controls = [](KeyboardInput& in) {
		if (in.mouseButton1)
		{
			heal_sound.Play(heal_sound_buf);
			return true;
		}
		if (in.p)
		{
			if(music_state != MUSIC_STATE::play)
			{
				music_sounds.Play(kwon_song);
				music_state = MUSIC_STATE::play;
				return true;
			}
			else if (music_state == MUSIC_STATE::play)
			{
				music_sounds.Pause(kwon_song);
				music_state = MUSIC_STATE::pause;
				return true;
			}
		}
		if (in.o)
		{
			if (music_state != MUSIC_STATE::stop)
			{
				music_sounds.Stop(kwon_song);
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
			master_volume+=.1f;
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
			master_volume-=.1f;
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

	//auto updateMusicStream = []()
	//{
	//	if (isPlaying)
	//	{
	//		music_sounds.UpdatePlayingBuffers(kwon_song);
	//	}
	//};
	//LOOP->addToSlowUpdate(updateMusicStream);

	return LOOP->runMainLoop();
}