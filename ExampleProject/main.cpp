#include <AncientArcher/AncientArcher.h>
#include <iostream>
#include "fps.h"
#include "setskybox.h"
#include "flashlight.h"
int main()
{
	AA::InitEngine();
	AA::SetClearColor(glm::vec3(0));
	static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());
	setfpscontrols(ourcam);
	setskybox();
	setupflashlight(ourcam);
	static int peasant_man = AA::AddObject("..\\ExampleProject\\res\\peasant_man.dae", ourcam, true);
	const float dist_from_us = 4.f;
	const float dist_down = -1.2f;
	AA::GetGameObject(peasant_man).SetTranslation(glm::vec3(0, dist_down, -dist_from_us));
	AA::SetDirectionalLight(glm::vec3(1.f), glm::vec3(.4f), glm::vec3(.4f), glm::vec3(1.f));
	AA::AddToDeltaUpdate([](float dt) {
		AA::GetGameObject(peasant_man).AddToRotation(glm::vec3(0, dt, 0));
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