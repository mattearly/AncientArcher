#pragma once
#include <AncientArcher/AncientArcher.h>
#include <iostream>

void setupflashlight(int ourcam)
{
	static int cam = ourcam;
	static bool flashlight_on = false;
	static int flashlight = -1;
	AA::AddToUpdate([]() {
		if (flashlight_on)
			AA::MoveSpotLight(flashlight, AA::GetCamera(cam).GetLocation(), *AA::GetCamera(cam).GetFront());
		});
	static float fl_inner_radius = glm::cos(glm::radians(4.5f));
	static float fl_outer_radius = glm::cos(glm::radians(24.5f));
	static float fl_constant = 1.f;
	static float fl_linear = .027f;
	static float fl_quad = .0028f;
	static glm::vec3 fl_ambient = glm::vec3(.2f);
	static glm::vec3 fl_diffuse = glm::vec3(.4f);
	static glm::vec3 fl_specular = glm::vec3(.1f);
	// toggle flashlight
	AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
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
					AA::GetCamera(cam).GetLocation(),
					*AA::GetCamera(cam).GetFront(),
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
		}
	);
}