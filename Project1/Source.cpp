#include <AncientArcher/AncientArcher.h>
#include <iostream>
#include "fpscontrols.h"
int main()
{
	AA::Init_Engine();

	static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());

	// first person controls
	setfpscontrols(ourcam);












	const std::string skyboxfolder = "E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\skybox\\";
	const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	const std::string skyboxfileext = ".jpg";
	std::vector<std::string> cubemapfiles;
	for (int j = 0; j < 6; ++j)
	{
		cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
	}
	const std::shared_ptr<AA::Skybox> skybox = std::make_shared<AA::Skybox>(cubemapfiles);
	AA::SetSkybox(skybox);

	static int enchantSound = AA::AddSoundEffect("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\enchant.ogg");
	static int windSound = AA::AddSoundEffect("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\wind.ogg");
	std::cout << "enchantSoundIndex " << enchantSound << '\n';
	std::cout << "windSoundIndex " << windSound << '\n';
	static int firstSpeaker = AA::AddSpeaker();
	std::cout << "speakerIndex " << firstSpeaker << '\n';
	
	AA::ChangeMusic("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\heroic_demise_music.ogg");

	AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));

	static int mutant = AA::AddObject("E:\\storage\\3d Models\\mutant.dae", ourcam, true);
	static int walking = AA::AddObject("E:\\storage\\3d Models\\Walking\\Walking.dae", ourcam, true);
	static int maria = AA::AddObject("E:\\storage\\3d Models\\maria_j_j_ong.dae", ourcam, true);
	static int peasant_man = AA::AddObject("E:\\storage\\3d Models\\peasant_man.dae", ourcam, true);

	const float dist_from_us = 2.f;
	const float dist_down = -1.2f;
	AA::GetGameObject(mutant).SetTranslation(glm::vec3(0, dist_down,-dist_from_us));
	AA::GetGameObject(walking).SetTranslation(glm::vec3(dist_from_us, dist_down, 0));
	AA::GetGameObject(walking).SetRotation(glm::vec3(0,-90, 0));
	AA::GetGameObject(maria).SetTranslation(glm::vec3(0, dist_down, dist_from_us));
	AA::GetGameObject(maria).SetRotation(glm::vec3(0,180, 0));	
	AA::GetGameObject(peasant_man).SetTranslation(glm::vec3(-dist_from_us, dist_down,0));
	AA::GetGameObject(peasant_man).SetRotation(glm::vec3(0,90, 0));

	static int main_spotlight = AA::AddSpotLight(glm::vec3(0), glm::vec3(0,0,1), glm::cos(glm::radians(0.5f)), glm::cos(glm::radians(30.5f)),
		1.f, .09f, .032f, glm::vec3(.4), glm::vec3(.4), glm::vec3(.1));

	AA::AddToDeltaUpdate([](float dt) 
		{
			AA::GetCamera(ourcam).ShiftYawAndPitch(15 * dt, 0);
			AA::MoveSpotLight(main_spotlight, AA::GetCamera(ourcam).GetLocation(), *AA::GetCamera(ourcam).GetFront());
		});

	static float vol = 1;
	AA::AddToKeyHandling([](AA::KeyboardInput& kb)
		{
			if (kb.mouseButton1)
			{
				AA::PlaySoundEffect(enchantSound, firstSpeaker);
			}
			if (kb.mouseButton2)
			{
				AA::PlaySoundEffect(windSound, firstSpeaker);
			}

			if (kb.minus)
			{
				float new_vol = vol - .1f;
				vol = new_vol;
				AA::GetSpeaker(firstSpeaker).SetVolume(new_vol);
				std::cout << "sound vol reduced to : " << new_vol << '\n';
			}
			if (kb.equal)
			{
				float new_vol = vol + .1f;
				vol = new_vol;
				AA::GetSpeaker(firstSpeaker).SetVolume(new_vol);
				std::cout << "sound vol increased to: " << new_vol << '\n';
			}

			if (kb.u)
			{
				std::cout << "playing state: " <<  AA::GetMusic().GetPlayingState() << '\n';
			}

		});

	AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
		if (kb.p)
		{
			//AA::GetMusic().SetLooping(true);
			AA::GetMusic().Play();
			return true;
		}
		if (kb.l)
		{
			AA::GetMusic().SetLooping(true);
			return true;
		}
		return false;
		});

	AA::AddToSlowUpdate([]() { AA::GetMusic().UpdatePlayBuffer();  });
	AA::Run();
}