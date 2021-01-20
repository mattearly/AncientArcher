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
		AA::Engine->GetGameObject(dovecote).setTranslation(glm::vec3(0, 0, 0));

		AA::Cam(cam).SetCurrentLocation(glm::vec3(0, 350, 300));
		//AA::Cam(cam).SetCurrentPitch(0.f);
		//AA::Cam(cam).SetCurrentYaw(0.f);
		AA::Cam(cam).SetMaxRenderDistance(2700.f);

		AA::Engine->AddToDeltaUpdate([](float dt) {
			AA::Engine->GetCamera(cam).ShiftCurrentLocation(glm::vec3(0, 0, dt * 200));
			if (AA::Engine->GetCamera(cam).GetLocation().z > 2300)
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

		AA::Engine->GetGameObject(wasd).setTranslation(glm::vec3(0, 0, 0));
		AA::Engine->GetGameObject(wasd).setScale(glm::vec3(3));
		//AA::Engine->GetGameObject(wasd).setRotation(glm::vec3(3));
		AA::Engine->GetGameObject(wasd).advanceRotation(glm::vec3(90,0,0));

		AA::Engine->GetCamera(cam).SetCurrentLocation(glm::vec3(0, 0, 30));
		AA::Engine->GetCamera(cam).SetCurrentPitch(0.f);
		AA::Engine->GetCamera(cam).SetCurrentYaw(270.f);

		static bool left, right, forward, backwards;
		static bool pressedleft, pressedright, pressedforward, pressedbackwards;

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

		static float VELOCITY = 100;

		AA::Engine->AddToDeltaUpdate([](float dt) {

			if (forward)
				AA::Engine->GetCamera(cam).ShiftCurrentLocation(glm::vec3(0, 0, -dt * VELOCITY));

			if (backwards)
				AA::Engine->GetCamera(cam).ShiftCurrentLocation(glm::vec3(0, 0, dt * VELOCITY));

			if (left)
				AA::Engine->GetCamera(cam).ShiftCurrentLocation(glm::vec3(-dt * VELOCITY, 0, 0));

			if (right)
				AA::Engine->GetCamera(cam).ShiftCurrentLocation(glm::vec3(dt * VELOCITY, 0, 0));

			// rotate object
			//AA::Engine->GetGameObject(wasd).advanceRotation(glm::vec3(dt * .2, dt * .2, dt * .2));
			AA::Engine->GetGameObject(wasd).advanceRotation(glm::vec3(0, dt * .2, 0));
			});

		AA::Engine->AddToSlowUpdate([]() {
			if (pressedleft && pressedright && pressedforward && pressedbackwards)
			{
				AA::Engine->Shutdown();
			}
			});

		Assert::AreEqual(AA::Engine->Run(), 0);
	}


};
}
