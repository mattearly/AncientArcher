#include <AncientArcher/AncientArcher.h>
#include <iostream>
#include "fpscontrols.h"
#include "setskybox.h"
#include "setupflashlight.h"
int main()
{
	AA::InitEngine();
	AA::SetClearColor(glm::vec3(0));
	static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());
	setfpscontrols(ourcam);
	//setskybox();
	setupflashlight(ourcam);
	//static int enchantSound = AA::AddSoundEffect("..\\AAUnitTest\\res\\enchant.ogg");
	//static int windSound = AA::AddSoundEffect("..\\AAUnitTest\\res\\wind.ogg");
	//static int firstSpeaker = AA::AddSpeaker();
	static int peasant_man = AA::AddObject("..\\AAUnitTest\\res\\peasant_man.dae", ourcam, true);
	const float dist_from_us = 4.f;
	const float dist_down = -1.2f;
	AA::GetGameObject(peasant_man).SetTranslation(glm::vec3(0, dist_down, -dist_from_us));
	AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.4f), glm::vec3(.4f), glm::vec3(1.f));
	AA::AddToDeltaUpdate([](float dt) {
		AA::GetGameObject(peasant_man).AddToRotation(glm::vec3(0, dt, 0));
		}
	);
	static float vol = 1;
	AA::AddToKeyHandling([](AA::KeyboardInput& kb) {
		//if (kb.mouseButton1)
			//AA::PlaySoundEffect(enchantSound, firstSpeaker);
		//if (kb.mouseButton2)
			//AA::PlaySoundEffect(windSound, firstSpeaker);
		//if (kb.minus)
		//{
		//	float new_vol = vol - .1f;
		//	vol = new_vol;
		//	AA::GetSpeaker(firstSpeaker).SetVolume(new_vol);
		//	std::cout << "sound vol reduced to : " << new_vol << '\n';
		//}
		//if (kb.equal)
		//{
		//	float new_vol = vol + .1f;
		//	vol = new_vol;
		//	AA::GetSpeaker(firstSpeaker).SetVolume(new_vol);
		//	std::cout << "sound vol increased to: " << new_vol << '\n';
		//}
		}
	);
	AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
		// turn directional light off
		if (kb.leftShift && kb.r) {
			AA::RemoveDirectionalLight();
			std::cout << "dir light turned off\n";
			return true;
		}
		// turn directional light on
		if (kb.r) {
			AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));
			std::cout << "dir light turned on\n";
			return true;
		}
		return false;
		}
	);
	return AA::Run();
}