#include <AncientArcher/AncientArcher.h>
#include <iostream>
#include "fpscontrols.h"
#include "setskybox.h"
int main()
{
	AA::InitEngine();
	static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());
	setfpscontrols(ourcam);
	//setskybox();
	static int enchantSound = AA::AddSoundEffect("..\\AAUnitTest\\res\\enchant.ogg");
	static int windSound = AA::AddSoundEffect("..\\AAUnitTest\\res\\wind.ogg");
	static int firstSpeaker = AA::AddSpeaker();
	static int peasant_man = AA::AddObject("..\\AAUnitTest\\res\\peasant_man.dae", ourcam, true);
	const float dist_from_us = 2.f;
	const float dist_down = -1.2f;
	AA::GetGameObject(peasant_man).SetTranslation(glm::vec3(0, dist_down, -dist_from_us));
	AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));

	AA::AddToDeltaUpdate([](float dt) {
			AA::GetGameObject(peasant_man).AddToRotation(glm::vec3(0, dt, 0));
		});

	static float vol = 1;
	AA::AddToKeyHandling([](AA::KeyboardInput& kb)
		{
			if (kb.mouseButton1)
				AA::PlaySoundEffect(enchantSound, firstSpeaker);
			if (kb.mouseButton2)
				AA::PlaySoundEffect(windSound, firstSpeaker);
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
		});


	static bool flashlight_on = false;
	static int flashlight = -1;
	AA::AddToUpdate([]() {
		if (flashlight_on)
			AA::MoveSpotLight(flashlight, AA::GetCamera(ourcam).GetLocation(), *AA::GetCamera(ourcam).GetFront());
		});
	static float fl_inner_radius = glm::cos(glm::radians(.5f));
	static float fl_outer_radius = glm::cos(glm::radians(30.5f));
	static float fl_constant = 1.f;
	static float fl_linear = .09f;
	static float fl_quad = .32f;
	static glm::vec3 fl_ambient = glm::vec3(.2f);
	static glm::vec3 fl_diffuse = glm::vec3(.4f);
	static glm::vec3 fl_specular = glm::vec3(.1f);
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
		// toggle flashlight
		if (kb.f)
		{
			if (flashlight_on)
			{
				AA::RemoveSpotLight(flashlight);
				flashlight_on = false;
				std::cout << "flashlight turned off\n";
			}
			else
			{
				flashlight = AA::AddSpotLight(
					AA::GetCamera(ourcam).GetLocation(),
					*AA::GetCamera(ourcam).GetFront(),
					fl_inner_radius,
					fl_outer_radius,
					fl_constant,
					fl_linear,
					fl_quad,
					fl_ambient,
					fl_diffuse,
					fl_specular
				);
				flashlight_on = true;
				std::cout << "flashlight turned on\n";
			}
			return true;
		}

		return false;
		});
	
	return AA::Run();
}