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

	Engine->AddToDeltaUpdate([](float dt) 
		{
			Engine->GetCamera(ourcam).ShiftYawAndPitch(5 * dt, 0);
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
		});

	Engine->Run();
}