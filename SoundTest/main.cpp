#include "../AAEngine/include/Loop.h"
#include "../AAEngine/include/Sound/SoundDevice.h"
#include "../AAEngine/include/Sound/SoundBufferManager.h"
#include "../AAEngine/include/Sound/SoundSource.h"
#include "../AAEngine/include/Sound/SoundStream.h"
#include <iostream>

using namespace AA;

int main()
{
	SoundDevice* sound_dev = SoundDevice::get(); // should init sound device and not throw if successful
	static uint32_t heal_sound_buf = SoundBufferManager::get()->addSoundEffect("../assets/sounds/heal.ogg");
	static SoundSource heal_sound;
	static SoundSource music_sounds;

	auto playDingOnClick = [](KeyboardInput& in) {
		if (in.mouseButton1)
		{
			heal_sound.Play(heal_sound_buf);
		}
	};
	LOOP->addToKeyHandling(playDingOnClick);

	  StreamPlayer* player;
	  int i;

	  player = NewPlayer();
	
	
	    if (!OpenPlayerFile(player, "E:/downloads/GojiBerryBeat-NanaKwabena.wav"))
	      std::cout << "could not open play file\n";
	    if (!StartPlayer(player))
	    {
	      ClosePlayerFile(player);
	      std::cout << "could not start playing music\n";
	    }
	
	    while (UpdatePlayer(player))
	      al_nssleep(10000000);
	
	    /* All done with this file. Close it and go to the next */
	    ClosePlayerFile(player);
	 

		std::cout << "done\n";

	  /* All files done. Delete the player, and close down OpenAL */
	  DeletePlayer(player);
	  player = NULL;
	
	  //CloseAL();

	//auto beginWithMusic = []()
	//{
	//	SoundBufferManager::get()->OpenPlayerFile("E:\\downloads\\GojiBerryBeat-NanaKwabena.wav");
	//	SoundBufferManager::get()->InitPlayer(music_sounds);
	//	SoundBufferManager::get()->StartPlayer(music_sounds);
	//};
	//LOOP->addToOnBegin(beginWithMusic);
	//auto updateMusicBuffer = []()
	//{
	//	SoundBufferManager::get()->UpdatePlayer(music_sounds);
	//};
	//LOOP->addToSlowUpdate(updateMusicBuffer);

	return LOOP->runMainLoop();
}