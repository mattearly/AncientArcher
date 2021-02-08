#include "pch.h"
#include "CppUnitTest.h"
#include "CppUnitTestLogger.h"
#include <AncientArcher.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AA;

namespace AAUnitTest
{
TEST_CLASS(AAUnitTest)
{
public:

	TEST_METHOD(A_BaseRun)
	{
		static float timeholder_A = 0.f;
		AA::Engine->AddToDeltaUpdate([](float dt)
			{
				timeholder_A += dt;
				if (timeholder_A > 1.f)
				{
					AA::Engine->Shutdown();
				}
			});
		Assert::AreEqual(AA::Engine->Run(), 0);
	}

	TEST_METHOD(B_BaseReset)
	{
		AA::Engine->SoftReset();

		static float timeholder_B = 0.f;

		AA::Engine->AddToDeltaUpdate([](float dt) {
			timeholder_B += dt;
			if (timeholder_B > 1.f)
			{
				AA::Engine->Shutdown();
			}
			});

		AA::Engine->AddToSlowUpdate([]() {AA::Engine->SetClearColor(glm::vec3(.4f, .4f, .4f)); });
		Assert::AreEqual(AA::Engine->Run(), 0);
	}

	TEST_METHOD(C_AutoCamAndModelTest)
	{
		AA::Engine->SoftReset();
		static int cam_C = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		// put in your own model to test it out
		static int dovecote = AA::Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam_C, shader);
		AA::Engine->GetGameObject(dovecote).SetTranslation(glm::vec3(0, 0, 0));

		AA::Cam(cam_C).SetCurrentLocation(glm::vec3(0, 350, 2300));
		AA::Cam(cam_C).SetMaxRenderDistance(2700.f);

		AA::Engine->AddToDeltaUpdate([](float dt) {
			float Velocity = 1200;
			AA::Cam(cam_C).ShiftCurrentLocation(glm::vec3(0, 0, -dt * Velocity));
			AA::Obj(dovecote).AddToRotation(glm::vec3(0, -dt, 0));
			if (AA::Cam(cam_C).GetLocation().z < 300)
			{
				AA::Engine->Shutdown();
			}
			});

		Assert::AreEqual(AA::Engine->Run(), 0);

	}

	TEST_METHOD(D_WASDMovementTest)
	{
		AA::Engine->SoftReset();
		static int cam_D = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		static int wasd = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\wasd.obj", cam_D, shader);

		AA::Engine->GetGameObject(wasd).SetScale(glm::vec3(3));
		AA::Engine->GetGameObject(wasd).AddToRotation(glm::vec3(90, 0, 0));

		AA::Cam(cam_D).SetCurrentLocation(glm::vec3(0, 0, 30));
		AA::Cam(cam_D).SetCurrentPitch(0.f);
		AA::Cam(cam_D).SetCurrentYaw(270.f);

		static bool left = false, right = false, forward = false, backwards = false;
		static bool pressedleft = false, pressedright = false, pressedforward = false, pressedbackwards = false;

		AA::Engine->AddToKeyHandling([](AA::KeyboardInput& kb) {
			if (kb.w)
				forward = pressedforward = true;
			else
				forward = false;

			if (kb.s)
				backwards = pressedbackwards = true;
			else
				backwards = false;

			if (kb.a)
				left = pressedleft = true;
			else
				left = false;

			if (kb.d)
				right = pressedright = true;
			else
				right = false;
			});

		static float VELOCITY = 10;

		AA::Engine->AddToDeltaUpdate([](float dt) {

			if (forward)
				AA::Cam(cam_D).ShiftCurrentLocation(glm::vec3(0, 0, -dt * VELOCITY));

			if (backwards)
				AA::Cam(cam_D).ShiftCurrentLocation(glm::vec3(0, 0, dt * VELOCITY));

			if (left)
				AA::Cam(cam_D).ShiftCurrentLocation(glm::vec3(-dt * VELOCITY, 0, 0));

			if (right)
				AA::Cam(cam_D).ShiftCurrentLocation(glm::vec3(dt * VELOCITY, 0, 0));

			// rotate object
			AA::Engine->GetGameObject(wasd).AddToRotation(glm::vec3(0, dt * .2, 0));
			});

		AA::Engine->AddToSlowUpdate([]() {
			if (pressedleft && pressedright && pressedforward && pressedbackwards)
			{
				AA::Engine->Shutdown();
			}
			});

		Assert::AreEqual(AA::Engine->Run(), 0);
	}

	TEST_METHOD(E_FirstPersonPerspectiveTest)
	{
		AA::Engine->SoftReset();

		static int cam_E = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		AA::Cam(cam_E).SetCurrentLocation(glm::vec3(0, 0, 0));
		AA::Cam(cam_E).ShiftYawAndPitch(90, 0);
		AA::Cam(cam_E).SetMaxRenderDistance(3000);

		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		static int skele = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\fpp_test_text.obj", cam_E, shader);
		AA::Engine->GetGameObject(skele).SetTranslation(glm::vec3(150, 0, -75));
		AA::Engine->GetGameObject(skele).SetScale(glm::vec3(10));
		AA::Engine->GetGameObject(skele).SetRotation(glm::vec3(0, glm::radians(-90.f), 0));

		static int negZ = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\-z.obj", cam_E, shader);
		AA::Obj(negZ).SetTranslation(glm::vec3(0, 75, -1500));
		AA::Obj(negZ).SetScale(glm::vec3(50));

		static int posZ = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\z.obj", cam_E, shader);
		AA::Obj(posZ).SetTranslation(glm::vec3(0, 75, 1500));
		AA::Obj(posZ).SetScale(glm::vec3(50));

		static int negX = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\-x.obj", cam_E, shader);
		AA::Obj(negX).SetTranslation(glm::vec3(-1500, 75, 0));
		AA::Obj(negX).SetScale(glm::vec3(50));

		static int posX = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\x.obj", cam_E, shader);
		AA::Obj(posX).SetTranslation(glm::vec3(1500, 75, 0));
		AA::Obj(posX).SetScale(glm::vec3(50));

		// First Person Mouse
		AA::Engine->SetCursorToDisabled();
		AA::Engine->SetReadMouseCurorAsFPP();
		AA::Engine->AddToMouseHandling([](AA::MouseInput& cursor) {
			AA::Cam(cam_E).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
			});

		static bool left = false, right = false, forward = false, backwards = false;
		AA::Engine->AddToKeyHandling([](AA::KeyboardInput& kb) {
			if (kb.w)
				forward = true;
			else
				forward = false;

			if (kb.s)
				backwards = true;
			else
				backwards = false;

			if (kb.a)
				left = true;
			else
				left = false;

			if (kb.d)
				right = true;
			else
				right = false;

			});
		AA::Engine->AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) -> bool {
			if (kb.esc) {
				//todo: options menu
				return true;
			}
			if (kb.f11)
			{
				//AA::Engine->ToggleFullscreen();
				return true;
			}
			return false;
			});

		static float VELOCITY = 2;

		AA::Engine->AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *AA::Cam(cam_E).GetFront();
			glm::vec3 right_vec = *AA::Cam(cam_E).GetRight();
			if (forward)
				AA::Cam(cam_E).ShiftCurrentLocation(front_vec * VELOCITY);

			if (backwards)
				AA::Cam(cam_E).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);

			if (left)
				AA::Cam(cam_E).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);

			if (right)
				AA::Cam(cam_E).ShiftCurrentLocation(right_vec * VELOCITY * .7f);

			//rotate x's and z's
			AA::Obj(negZ).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			AA::Obj(posZ).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			AA::Obj(negX).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			AA::Obj(posX).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));

			});

		Assert::AreEqual(AA::Engine->Run(), 0);
	}

	TEST_METHOD(F_CustomShaderTest)
	{
		AA::Engine->SoftReset();
		static int cam_F = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader("..\\..\\AAUnitTest\\res\\basicvert.glsl", "..\\..\\AAUnitTest\\res\\basicfrag.glsl", true);

		// put in your own model to test it out
		int dovecote = AA::Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam_F, shader);
		AA::Engine->GetGameObject(dovecote).SetTranslation(glm::vec3(0, -200, 0));

		int ground = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\cube_stretched.obj", cam_F, shader);
		Obj(ground).SetScale(glm::vec3(100, 1, 100));
		Obj(ground).SetTranslation(glm::vec3(0, -300, 0));

		AA::Cam(cam_F).SetCurrentLocation(glm::vec3(0, 0, 1600));
		AA::Cam(cam_F).SetMaxRenderDistance(2700.f);

		static bool up, down, left, right;
		Engine->AddToKeyHandling([](auto& kb) {
			if (kb.upArrow)
				up = true;
			else
				up = false;

			if (kb.downArrow)
				down = true;
			else
				down = false;

			if (kb.leftArrow)
				left = true;
			else
				left = false;

			if (kb.rightArrow)
				right = true;
			else
				right = false;
			});

		AA::Engine->AddToDeltaUpdate([](float dt) {
			float Velocity = 200;
			if (up)
				Cam(cam_F).ShiftCurrentLocation(glm::vec3(0, dt * Velocity, 0));
			if (down)
				Cam(cam_F).ShiftCurrentLocation(glm::vec3(0, -dt * Velocity, 0));
			if (left)
				Cam(cam_F).ShiftCurrentLocation(glm::vec3(-dt * Velocity, 0, 0));
			if (right)
				Cam(cam_F).ShiftCurrentLocation(glm::vec3(dt * Velocity, 0, 0));
			});

		Engine->AddToTimedOutKeyHandling([](auto& kb) {
			if (kb.p)
			{
				Cam(cam_F).SetToPerspective();
				return true;
			}

			if (kb.o)
			{
				Cam(cam_F).__setToOrtho();
				return true;
			}

			return false;
			});

		Assert::AreEqual(AA::Engine->Run(), 0);

	}

	TEST_METHOD(G_SkyboxTest)
	{
		AA::Engine->SoftReset();

		static int cam_G = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		AA::Cam(cam_G).SetCurrentLocation(glm::vec3(0, 0, 0));
		AA::Cam(cam_G).ShiftYawAndPitch(90, 0);
		AA::Cam(cam_G).SetMaxRenderDistance(3000);

		const std::string skyboxfolder = "..\\..\\AAUnitTest\\res\\skybox\\";
		const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
		const std::string skyboxfileext = ".jpg";
		std::vector<std::string> cubemapfiles;
		for (int j = 0; j < 6; ++j)
		{
			cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
		}
		const std::shared_ptr<AA::Skybox> skybox = std::make_shared<AA::Skybox>(cubemapfiles);
		AA::Engine->SetSkybox(skybox);

		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		int plane_thing = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\cube_stretched.obj", cam_G, shader);
		Obj(plane_thing).SetScale(glm::vec3(11.f, .2f, 11.f));
		Obj(plane_thing).SetTranslation(glm::vec3(0, -20, 0));

		// First Person Mouse
		AA::Engine->SetCursorToDisabled();
		AA::Engine->SetReadMouseCurorAsFPP();
		AA::Engine->AddToMouseHandling([](AA::MouseInput& cursor) {
			AA::Cam(cam_G).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
			});

		static bool left = false, right = false, forward = false, backwards = false;
		AA::Engine->AddToKeyHandling([](AA::KeyboardInput& kb) {
			if (kb.w)
				forward = true;
			else
				forward = false;

			if (kb.s)
				backwards = true;
			else
				backwards = false;

			if (kb.a)
				left = true;
			else
				left = false;

			if (kb.d)
				right = true;
			else
				right = false;

			});

		static float VELOCITY = 2;

		AA::Engine->AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *AA::Cam(cam_G).GetFront();
			glm::vec3 right_vec = *AA::Cam(cam_G).GetRight();
			if (forward)
				AA::Cam(cam_G).ShiftCurrentLocation(front_vec * VELOCITY);

			if (backwards)
				AA::Cam(cam_G).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);

			if (left)
				AA::Cam(cam_G).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);

			if (right)
				AA::Cam(cam_G).ShiftCurrentLocation(right_vec * VELOCITY * .7f);

			});

		Assert::AreEqual(AA::Engine->Run(), 0);
	}
};
}
