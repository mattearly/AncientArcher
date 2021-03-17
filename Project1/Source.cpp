#include <AncientArcher/AncientArcher.h>
#include <iostream>
using namespace AA;
int main()
{
	static int ourcam = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());

	const std::string skyboxfolder = "E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\skybox\\";
	const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	const std::string skyboxfileext = ".jpg";
	std::vector<std::string> cubemapfiles;
	for (int j = 0; j < 6; ++j)
	{
		cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
	}
	const std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>(cubemapfiles);
	Engine->SetSkybox(skybox);

	static int enchantSound = Engine->AddSoundEffect("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\enchant.ogg");
	static int windSound = Engine->AddSoundEffect("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\wind.ogg");
	std::cout << "enchantSoundIndex " << enchantSound << '\n';
	std::cout << "windSoundIndex " << windSound << '\n';
	static int firstSpeaker = Engine->AddSpeaker();
	std::cout << "speakerIndex " << firstSpeaker << '\n';
	
	Engine->ChangeMusic("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\heroic_demise_music.ogg");

	Engine->SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));

	static int mutant = Engine->AddObject("E:\\storage\\3d Models\\mutant.dae", ourcam, true);
	static int walking = Engine->AddObject("E:\\storage\\3d Models\\Walking\\Walking.dae", ourcam, true);
	static int maria = Engine->AddObject("E:\\storage\\3d Models\\maria_j_j_ong.dae", ourcam, true);
	static int peasant_man = Engine->AddObject("E:\\storage\\3d Models\\peasant_man.dae", ourcam, true);

	const float dist_from_us = 2.f;
	const float dist_down = -1.2f;
	Obj(mutant).SetTranslation(glm::vec3(0, dist_down,-dist_from_us));
	Obj(walking).SetTranslation(glm::vec3(dist_from_us, dist_down, 0));
	Obj(walking).SetRotation(glm::vec3(0,-90, 0));
	Obj(maria).SetTranslation(glm::vec3(0, dist_down, dist_from_us));
	Obj(maria).SetRotation(glm::vec3(0,180, 0));	
	Obj(peasant_man).SetTranslation(glm::vec3(-dist_from_us, dist_down,0));
	Obj(peasant_man).SetRotation(glm::vec3(0,90, 0));

	static int main_spotlight = Engine->AddSpotLight(glm::vec3(0), glm::vec3(0,0,1), glm::cos(glm::radians(0.5f)), glm::cos(glm::radians(30.5f)),
		1.f, .09f, .032f, glm::vec3(.4), glm::vec3(.4), glm::vec3(.1));

	Engine->AddToDeltaUpdate([](float dt) 
		{
			Engine->GetCamera(ourcam).ShiftYawAndPitch(15 * dt, 0);
			Engine->MoveSpotLight(main_spotlight, Cam(ourcam).GetLocation(), *Cam(ourcam).GetFront());
		});

	static float vol = 1;
	Engine->AddToKeyHandling([](KeyboardInput& kb)
		{
			if (kb.mouseButton1)
			{
				Engine->PlaySoundEffect(enchantSound, firstSpeaker);
			}
			if (kb.mouseButton2)
			{
				Engine->PlaySoundEffect(windSound, firstSpeaker);
			}

			if (kb.minus)
			{
				float new_vol = vol - .1f;
				vol = new_vol;
				Engine->GetSpeaker(firstSpeaker).SetVolume(new_vol);
				std::cout << "sound vol reduced to : " << new_vol << '\n';
			}
			if (kb.equal)
			{
				float new_vol = vol + .1f;
				vol = new_vol;
				Engine->GetSpeaker(firstSpeaker).SetVolume(new_vol);
				std::cout << "sound vol increased to: " << new_vol << '\n';
			}

			if (kb.u)
			{
				std::cout << "playing state: " <<  Engine->GetMusic().GetPlayingState() << '\n';
			}

		});

	Engine->AddToTimedOutKeyHandling([](KeyboardInput& kb) {
		if (kb.p)
		{
			//Engine->GetMusic().SetLooping(true);
			Engine->GetMusic().Play();
			return true;
		}
		if (kb.l)
		{
			Engine->GetMusic().SetLooping(true);
			return true;
		}
		return false;
		});

	Engine->AddToSlowUpdate([]() { Engine->GetMusic().UpdatePlayBuffer();  });

	Engine->Run();
}