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
			if (timeholder_B > 3.f)
			{
				AA::Engine->Shutdown();
			}
			});
		Assert::AreEqual(AA::Engine->Run(), 0);
	}

	TEST_METHOD(C_BaseCamAndModelTest)
	{
		AA::Engine->SoftReset();
		static int cam = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		int dovecote = AA::Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam, shader);
		AA::Engine->GetGameObject(dovecote).setTranslation(glm::vec3(0, 0, 0));

		AA::Engine->GetCamera(cam).setCurrentPosition(glm::vec3(0, 450, 300));
		AA::Engine->GetCamera(cam).setCurrentPitch(0.f);
		AA::Engine->GetCamera(cam).setCurrentYaw(270.f);

		AA::Engine->AddToDeltaUpdate([](float dt) {
			AA::Engine->GetCamera(cam).shiftCurrentPosition(glm::vec3(0, 0, dt * 200));
			if (AA::Engine->GetCamera(cam).getLocation().z > 2300)
			{
				AA::Engine->Shutdown();
			}
			});

		Assert::AreEqual(AA::Engine->Run(), 0);

	}

	TEST_METHOD(D_BaseMovementTest)
	{
		AA::Engine->SoftReset();
		static int cam = AA::Engine->AddCamera(AA::Engine->GetWindowWidth(), AA::Engine->GetWindowHeight());
		int shader = AA::Engine->AddShader(AA::SHADERTYPE::DIFF);

		int dovecote = AA::Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam, shader);
		AA::Engine->GetGameObject(dovecote).setTranslation(glm::vec3(0, 0, 0));

		AA::Engine->GetCamera(cam).setCurrentPosition(glm::vec3(0, 350, 700));
		AA::Engine->GetCamera(cam).setCurrentPitch(0.f);
		AA::Engine->GetCamera(cam).setCurrentYaw(270.f);

		static bool left, right, forward, backwards;

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
		static float VELOCITY = 100;
		AA::Engine->AddToDeltaUpdate([](float dt) {
			if (forward)
				AA::Engine->GetCamera(cam).shiftCurrentPosition(glm::vec3(0, 0, -dt * VELOCITY));
			if (backwards)
				AA::Engine->GetCamera(cam).shiftCurrentPosition(glm::vec3(0, 0, dt * VELOCITY));
			if (left)
				AA::Engine->GetCamera(cam).shiftCurrentPosition(glm::vec3(-dt * VELOCITY, 0, 0));
			if (right)
				AA::Engine->GetCamera(cam).shiftCurrentPosition(glm::vec3(dt * VELOCITY, 0, 0));

			});

		Assert::AreEqual(AA::Engine->Run(), 0);
	}


};
}
