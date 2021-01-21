#include "pch.h"
#include "CppUnitTest.h"
#include "CppUnitTestLogger.h"
#include <AncientArcher.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
				if (timeholder_A > 3.f)
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
			if (timeholder_B > 6.f)
			{
				AA::Engine->Shutdown();
			}
			});
		Assert::AreEqual(AA::Engine->Run(), 0);
	}

	TEST_METHOD(C_AutoCamAndModelTest)
	{
		AA::Engine->SoftReset();
		static int cam = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		// this is local, put in your own model to test it out
		int dovecote = AA::Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam, shader);
		AA::Engine->GetGameObject(dovecote).SetTranslation(glm::vec3(0, 0, 0));

		AA::Cam(cam).SetCurrentLocation(glm::vec3(0, 350, 300));
		//AA::Cam(cam).SetCurrentPitch(0.f);
		//AA::Cam(cam).SetCurrentYaw(0.f);
		AA::Cam(cam).SetMaxRenderDistance(2700.f);

		AA::Engine->AddToDeltaUpdate([](float dt) {
			AA::Cam(cam).ShiftCurrentLocation(glm::vec3(0, 0, dt * 200));
			if (AA::Cam(cam).GetLocation().z > 2300)
			{
				AA::Engine->Shutdown();
			}
			});

		Assert::AreEqual(AA::Engine->Run(), 0);

	}

	TEST_METHOD(D_WASDMovementTest)
	{
		AA::Engine->SoftReset();
		static int cam = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		static int wasd = AA::Engine->AddObject("..\\..\\AAUnitTest\\res\\wasd.obj", cam, shader);

		AA::Engine->GetGameObject(wasd).SetTranslation(glm::vec3(0, 0, 0));
		AA::Engine->GetGameObject(wasd).SetScale(glm::vec3(3));
		//AA::Engine->GetGameObject(wasd).SetRotation(glm::vec3(3));
		AA::Engine->GetGameObject(wasd).AddToRotation(glm::vec3(90, 0, 0));

		AA::Cam(cam).SetCurrentLocation(glm::vec3(0, 0, 30));
		AA::Cam(cam).SetCurrentPitch(0.f);
		AA::Cam(cam).SetCurrentYaw(270.f);

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
				AA::Cam(cam).ShiftCurrentLocation(glm::vec3(0, 0, -dt * VELOCITY));

			if (backwards)
				AA::Cam(cam).ShiftCurrentLocation(glm::vec3(0, 0, dt * VELOCITY));

			if (left)
				AA::Cam(cam).ShiftCurrentLocation(glm::vec3(-dt * VELOCITY, 0, 0));

			if (right)
				AA::Cam(cam).ShiftCurrentLocation(glm::vec3(dt * VELOCITY, 0, 0));

			// rotate object
			//AA::Engine->GetGameObject(wasd).AddToRotation(glm::vec3(dt * .2, dt * .2, dt * .2));
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

		static int cam = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);
		// add your own model here
		static int skele = AA::Engine->AddObject("E:\\storage\\Models\\fgc_skeleton.fbx", cam, shader);

		AA::Cam(cam).SetCurrentLocation(glm::vec3(0, 0, 0));
		AA::Cam(cam).SetMaxRenderDistance(3000);
		// config engine loop for first person perspective view and controls
		AA::Engine->SetCursorToDisabled();
		AA::Engine->SetReadMouseCurorAsFPP();

		AA::Engine->GetGameObject(skele).SetTranslation(glm::vec3(0, -100, -1000));
		AA::Engine->GetGameObject(skele).SetScale(glm::vec3(.5));
		//AA::Engine->GetGameObject(skele).SetRotation(glm::vec3(3));
		//AA::Engine->GetGameObject(skele).AddToRotation(glm::vec3(90, 0, 0));

		//AA::Cam(cam).SetCurrentPitch(0.f);
		//AA::Cam(cam).SetCurrentYaw(270.f);

		AA::Engine->AddToMouseHandling([](AA::MouseInput& cursor) {
			AA::Cam(cam).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
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
		AA::Engine->AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
			if (kb.leftAlt && kb.enter) {
				AA::Engine->ToggleFullscreen();
				return true;
			}
			return false;
			});

		static float VELOCITY = 7;

		AA::Engine->AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *AA::Cam(cam).GetFront();
			glm::vec3 right_vec = *AA::Cam(cam).GetRight();
			if (forward)
				AA::Cam(cam).ShiftCurrentLocation(front_vec * VELOCITY);

			if (backwards)
				AA::Cam(cam).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);

			if (left)
				AA::Cam(cam).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);

			if (right)
				AA::Cam(cam).ShiftCurrentLocation(right_vec * VELOCITY * .7f);
			});


		Assert::AreEqual(AA::Engine->Run(), 0);
	}

};
}
