#include <AncientArcher/AncientArcher.h>
#include <iostream>
#include "fpscontrols.h"
int main()
{
	AA::InitEngine();

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

	//AA::ChangeMusic("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\heroic_demise_music.ogg");


	static int mutant = AA::AddObject("E:\\storage\\3d Models\\mutant.dae", ourcam, true);
	static int walking = AA::AddObject("E:\\storage\\3d Models\\Walking\\Walking.dae", ourcam, true);
	static int maria = AA::AddObject("E:\\storage\\3d Models\\maria_j_j_ong.dae", ourcam, true);
	static int peasant_man = AA::AddObject("E:\\storage\\3d Models\\peasant_man.dae", ourcam, true);
	static int test_wall = AA::AddObject("E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\test_wall.dae", ourcam, true);

	const float dist_from_us = 2.f;
	const float dist_down = -1.2f;
	AA::GetGameObject(mutant).SetTranslation(glm::vec3(0, dist_down, -dist_from_us));
	AA::GetGameObject(walking).SetTranslation(glm::vec3(dist_from_us, dist_down, 0));
	AA::GetGameObject(walking).SetRotation(glm::vec3(0, -90, 0));
	AA::GetGameObject(maria).SetTranslation(glm::vec3(0, dist_down, dist_from_us));
	AA::GetGameObject(maria).SetRotation(glm::vec3(0, 180, 0));
	AA::GetGameObject(peasant_man).SetTranslation(glm::vec3(-dist_from_us, dist_down, 0));
	AA::GetGameObject(peasant_man).SetRotation(glm::vec3(0, 90, 0));

	AA::GetGameObject(test_wall).SetTranslation(glm::vec3(0, dist_down, -10));
	AA::GetGameObject(test_wall).SetRotation(glm::vec3(0, glm::radians(90.f), 0));

	static int main_spotlight = AA::AddSpotLight(glm::vec3(0), glm::vec3(0, 0, 1), glm::cos(glm::radians(0.5f)), glm::cos(glm::radians(30.5f)),
		1.f, .09f, .032f, glm::vec3(.4), glm::vec3(.4), glm::vec3(.1));

	//static int second_spotlight = AA::AddSpotLight(glm::vec3(0), glm::vec3(0, 0, 1), glm::cos(glm::radians(0.5f)), glm::cos(glm::radians(30.5f)),
	//	1.f, .09f, .032f, glm::vec3(.4), glm::vec3(.4), glm::vec3(.1));
	static int main_point_light = AA::AddPointLight(glm::vec3(0.f), 1.f, .09f, .032f, glm::vec3(.8f),
		glm::vec3(.8f), glm::vec3(.5f));

	AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));

	static bool flashlight_on = true;
	AA::AddToDeltaUpdate([](float dt)
		{
			//AA::GetCamera(ourcam).ShiftYawAndPitch(15 * dt, 0);  // the spin

			if (flashlight_on)
				AA::MoveSpotLight(main_spotlight, AA::GetCamera(ourcam).GetLocation(), *AA::GetCamera(ourcam).GetFront());

			static float curr_rot = 0;
			curr_rot += dt * .1;
			if (curr_rot > 360)
				curr_rot = 0;

			static float curr_amb = .1f;
			static bool amb_going_up = true;  // to oscilate
			if (amb_going_up)
				curr_amb += dt * .5f;
			else
				curr_amb -= dt * .5f;
			if (curr_amb > 1.f)
				amb_going_up = false;
			if (curr_amb < .1f)
				amb_going_up = true;

			static float curr_diff = .3f;
			static bool diff_going_up = true;
			if (diff_going_up)
				curr_diff += dt * .5f;
			else
				curr_diff -= dt * .5f;

			if (curr_diff > 1.f)
				diff_going_up = false;
			if (curr_diff < .3f)
				diff_going_up = true;

			//AA::ChangeSpotLight(second_spotlight, glm::vec3(0), glm::vec3(cos(curr_rot), 0, sin(curr_rot)), glm::cos(glm::radians(0.5f)), glm::cos(glm::radians(30.5f)),
			//	1.f, .09f, .032f, glm::vec3(curr_amb), glm::vec3(curr_diff), glm::vec3(.1));


			//AA::MovePointLight(main_point_light, AA::GetCamera(ourcam).GetLocation());

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
				std::cout << "playing state: " << AA::GetMusic().GetPlayingState() << '\n';
			}


		});

	static glm::vec3 main_point_diff = glm::vec3(.3f);

	AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
		//if (kb.p)
		//{
		//	//AA::GetMusic().SetLooping(true);
		//	//AA::GetMusic().Play();
		//	return true;
		//}
		//if (kb.l)
		//{
		//	return true;
		//}
		if (kb.r)
		{
			AA::RemoveDirectionalLight();
		}
		if (kb.leftShift && kb.r)
		{
			AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));
		}
		if (kb.f)
		{
			if (flashlight_on)
			{
				AA::RemoveSpotLight(main_spotlight);
				flashlight_on = false;
				return true;
			}
			else
			{
				main_spotlight = AA::AddSpotLight(glm::vec3(0), glm::vec3(0, 0, 1), glm::cos(glm::radians(0.5f)), glm::cos(glm::radians(30.5f)),
					1.f, .09f, .032f, glm::vec3(.4), glm::vec3(.4), glm::vec3(.1));
				flashlight_on = true;
				return true;
			}
		}
		if (kb.leftShift && kb.leftArrow)
		{
			if (main_point_light == -1)
				return false;

			if (main_point_diff.x > 0.f)
				main_point_diff.x -= .1f;
			else
				main_point_diff.x = 0.f;

			AA::ChangePointLight(main_point_light, glm::vec3(0.f), 1.f, .09f, .032f,
				/*ambient*/ glm::vec3(.5f),
				/*diffuse*/ main_point_diff,
				glm::vec3(.5f));
			return true;
		}

		if (kb.leftShift && kb.rightArrow)
		{
			if (main_point_light == -1)
				return false;

			if (main_point_diff.x < 1.f)
				main_point_diff.x += .1f;
			else
				main_point_diff.x = 1.f;
			AA::ChangePointLight(main_point_light, glm::vec3(0.f), 1.f, .09f, .032f,
				/*ambient*/ glm::vec3(.5f),
				/*diffuse*/ main_point_diff,
				glm::vec3(.5f));
			return true;
		}

		if (kb.n)
		{
			// toggle point light
			if (main_point_light != -1)
			{
				bool success = AA::RemovePointLight(main_point_light);
				if (success)
					main_point_light = -1;
				return true;
			}
			else if (main_point_light == -1)
			{
				main_point_light = AA::AddPointLight(glm::vec3(0.f), 1.f, .09f, .032f, glm::vec3(.8f),
					glm::vec3(.8f), glm::vec3(.5f));
				return true;
			}
		}
		return false;
		});

	//AA::AddToSlowUpdate([]() { AA::GetMusic().UpdatePlayBuffer(); });
	return AA::Run();
}