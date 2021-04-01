#include "pch.h"
#include "CppUnitTest.h"
#include "CppUnitTestLogger.h"
#include <AncientArcher/AncientArcher.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AA;
namespace AAUnitTest
{

int test_camera = 0;
float timeholder_1 = 0.f;
TEST_CLASS(AAUnitTest)
{
public:
	TEST_METHOD(A_BaseRun)
	{
		SetWindowTitle("A_BaseRun");
		timeholder_1 = 0.f;
		AddToDeltaUpdate([](float dt)
			{
				timeholder_1 += dt;
				if (timeholder_1 > 2.f)
				{
					Shutdown();
				}
			});
		Assert::AreEqual(Run(), 0);
		// RUN 2
		SoftReset();
		SetWindowTitle("A_BaseRun_2");
		timeholder_1 = 0.f;
		AddToDeltaUpdate([](float dt) {
			timeholder_1 += dt;
			if (timeholder_1 > 2.f)
			{
				Shutdown();
			}
			});
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(B_AutoCamAndModelTest)
	{
		SoftReset();
		SetWindowTitle("B_AutoCamAndModelTest");
		test_camera = AddCamera(GetWindowWidth(), GetWindowHeight());
		static int man = AddObject("..\\..\\AAUnitTest\\res\\peasant_man.dae", test_camera, false);
		GetGameObject(man).SetTranslation(glm::vec3(0, 0, 0));
		GetCamera(test_camera).SetCurrentLocation(glm::vec3(0, 350, 2300));
		GetCamera(test_camera).SetMaxRenderDistance(2700.f);
		AddToDeltaUpdate([](float dt) {
			float Velocity = 1200;
			GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(0, 0, -dt * Velocity));
			GetGameObject(man).AddToRotation(glm::vec3(0, -dt, 0));
			if (GetCamera(test_camera).GetLocation().z < 300)
			{
				Shutdown();
			}
			});
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(C_WASDMovementTest)
	{
		SoftReset();
		SetWindowTitle("C_WASDMovementTest");
		test_camera = AddCamera(GetWindowWidth(), GetWindowHeight());
		static int wasd = AddObject("..\\..\\AAUnitTest\\res\\wasd.obj", test_camera, false);
		GetGameObject(wasd).SetScale(glm::vec3(3));
		GetGameObject(wasd).AddToRotation(glm::vec3(90, 0, 0));
		GetCamera(test_camera).SetCurrentLocation(glm::vec3(0, 0, 30));
		GetCamera(test_camera).SetCurrentPitch(0.f);
		GetCamera(test_camera).SetCurrentYaw(270.f);
		static bool left = false, right = false, forward = false, backwards = false;
		static bool pressedleft = false, pressedright = false, pressedforward = false, pressedbackwards = false;
		AddToKeyHandling([](KeyboardInput& kb) {
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
		AddToDeltaUpdate([](float dt) {
			if (forward)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(0, 0, -dt * VELOCITY));
			if (backwards)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(0, 0, dt * VELOCITY));
			if (left)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(-dt * VELOCITY, 0, 0));
			if (right)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(dt * VELOCITY, 0, 0));
			// rotate object
			GetGameObject(wasd).AddToRotation(glm::vec3(0, dt * .2, 0));
			});
		AddToSlowUpdate([]() {
			if (pressedleft && pressedright && pressedforward && pressedbackwards)
			{
				Shutdown();
			}
			});
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(D_FirstPersonPerspectiveTest)
	{
		SoftReset();
		SetWindowTitle("D_FirstPersonPerspectiveTest");
		test_camera = AddCamera(GetWindowWidth(), GetWindowHeight());
		GetCamera(test_camera).SetCurrentLocation(glm::vec3(0, 0, 0));
		GetCamera(test_camera).ShiftYawAndPitch(90, 0);
		GetCamera(test_camera).SetMaxRenderDistance(3000);
		static int fpp_test_text = AddObject("..\\..\\AAUnitTest\\res\\fpp_test_text.obj", test_camera, false);
		GetGameObject(fpp_test_text).SetTranslation(glm::vec3(150, 0, -75));
		GetGameObject(fpp_test_text).SetScale(glm::vec3(10));
		GetGameObject(fpp_test_text).SetRotation(glm::vec3(0, glm::radians(-90.f), 0));
		static int negZ = AddObject("..\\..\\AAUnitTest\\res\\-z.obj", test_camera, false);
		GetGameObject(negZ).SetTranslation(glm::vec3(0, 75, -1500));
		GetGameObject(negZ).SetScale(glm::vec3(50));
		static int posZ = AddObject("..\\..\\AAUnitTest\\res\\z.obj", test_camera, false);
		GetGameObject(posZ).SetTranslation(glm::vec3(0, 75, 1500));
		GetGameObject(posZ).SetScale(glm::vec3(50));
		static int negX = AddObject("..\\..\\AAUnitTest\\res\\-x.obj", test_camera, false);
		GetGameObject(negX).SetTranslation(glm::vec3(-1500, 75, 0));
		GetGameObject(negX).SetScale(glm::vec3(50));
		static int posX = AddObject("..\\..\\AAUnitTest\\res\\x.obj", test_camera, false);
		GetGameObject(posX).SetTranslation(glm::vec3(1500, 75, 0));
		GetGameObject(posX).SetScale(glm::vec3(50));
		// First Person Mouse
		SetCursorToDisabled();
		SetReadMouseCurorAsFPP();
		AddToMouseHandling([](MouseInput& cursor) {
			GetCamera(test_camera).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
			});
		static bool left = false, right = false, forward = false, backwards = false;
		AddToKeyHandling([](KeyboardInput& kb) {
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
		AddToTimedOutKeyHandling([](KeyboardInput& kb) -> bool {
			if (kb.f11)
			{
				//todo: toggle fullscreen
				return true;
			}
			if (kb.p)
			{
				//todo: set to perspective
				GetCamera(test_camera).SetToPerspective();
				return true;
			}
			if (kb.o)
			{
				//todo: set to ortho
				GetCamera(test_camera).__setToOrtho();
				return true;
			}
			return false;
			});
		static float VELOCITY = 2;
		AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *GetCamera(test_camera).GetFront();
			glm::vec3 right_vec = *GetCamera(test_camera).GetRight();
			if (forward)
				GetCamera(test_camera).ShiftCurrentLocation(front_vec * VELOCITY);
			if (backwards)
				GetCamera(test_camera).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);
			if (left)
				GetCamera(test_camera).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);
			if (right)
				GetCamera(test_camera).ShiftCurrentLocation(right_vec * VELOCITY * .7f);
			//rotate x's and z's
			GetGameObject(negZ).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			GetGameObject(posZ).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			GetGameObject(negX).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			GetGameObject(posX).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			});
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(E_CustomShaderTest)
	{
		SoftReset();
		SetWindowTitle("E_CustomShaderTest");
		test_camera = AddCamera(GetWindowWidth(), GetWindowHeight());
		// put in your own model to test it outs
		int dovecote = AddObject("E:\\storage\\Models\\dovecote.fbx", test_camera, false);
		GetGameObject(dovecote).SetTranslation(glm::vec3(0, -200, 0));
		int ground = AddObject("..\\..\\AAUnitTest\\res\\cube_stretched.obj", test_camera, false);
		GetGameObject(ground).SetScale(glm::vec3(100, 1, 100));
		GetGameObject(ground).SetTranslation(glm::vec3(0, -300, 0));
		GetCamera(test_camera).SetCurrentLocation(glm::vec3(0, 0, 1600));
		GetCamera(test_camera).SetMaxRenderDistance(2700.f);
		static bool up = false, down = false, left = false, right = false;
		AddToKeyHandling([](auto& kb) {
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
		AddToDeltaUpdate([](float dt) {
			float Velocity = 200;
			if (up)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(0, dt * Velocity, 0));
			if (down)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(0, -dt * Velocity, 0));
			if (left)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(-dt * Velocity, 0, 0));
			if (right)
				GetCamera(test_camera).ShiftCurrentLocation(glm::vec3(dt * Velocity, 0, 0));
			});
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(F_SkyboxTest)
	{
		SoftReset();
		SetWindowTitle("F_SkyboxTest");
		test_camera = AddCamera(GetWindowWidth(), GetWindowHeight());
		GetCamera(test_camera).SetCurrentLocation(glm::vec3(0, 0, 0));
		GetCamera(test_camera).ShiftYawAndPitch(90, 0);
		GetCamera(test_camera).SetMaxRenderDistance(3000);
		const std::string skyboxfolder = "..\\..\\AAUnitTest\\res\\skybox\\";
		const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
		const std::string skyboxfileext = ".jpg";
		std::vector<std::string> cubemapfiles;
		for (int j = 0; j < 6; ++j)
		{
			cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
		}
		const std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>(cubemapfiles);
		SetSkybox(skybox);
		static int fpp_test_text = AddObject("..\\..\\AAUnitTest\\res\\fpp_test_text.obj", test_camera, false);
		GetGameObject(fpp_test_text).SetTranslation(glm::vec3(150, 0, -75));
		GetGameObject(fpp_test_text).SetScale(glm::vec3(10));
		GetGameObject(fpp_test_text).SetRotation(glm::vec3(0, glm::radians(-90.f), 0));
		int plane_thing = AddObject("..\\..\\AAUnitTest\\res\\cube_stretched.obj", test_camera, false);
		GetGameObject(plane_thing).SetScale(glm::vec3(11.f, .2f, 11.f));
		GetGameObject(plane_thing).SetTranslation(glm::vec3(0, -20, 0));
		// First Person Mouse
		SetCursorToDisabled();
		SetReadMouseCurorAsFPP();
		AddToMouseHandling([](MouseInput& cursor) {
			GetCamera(test_camera).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
			});
		static bool left = false, right = false, forward = false, backwards = false;
		AddToKeyHandling([](KeyboardInput& kb) {
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
		AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *GetCamera(test_camera).GetFront();
			glm::vec3 right_vec = *GetCamera(test_camera).GetRight();
			if (forward)
				GetCamera(test_camera).ShiftCurrentLocation(front_vec * VELOCITY);
			if (backwards)
				GetCamera(test_camera).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);
			if (left)
				GetCamera(test_camera).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);
			if (right)
				GetCamera(test_camera).ShiftCurrentLocation(right_vec * VELOCITY * .7f);
			});
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(G_SoundTest)
	{
		SoftReset();
		SetWindowTitle("G_SoundTest");
		static auto speaker = AddSpeaker();
		static auto MyWindSound = AddSoundEffect("..\\..\\AAUnitTest\\res\\wind.ogg");
		AddToOnBegin([]() {GetSpeaker(speaker).Play(MyWindSound); });
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(H_LongSoundTest)
	{
		SoftReset();
		SetWindowTitle("H_LongSoundTest");
		ChangeMusic("..\\..\\AAUnitTest\\res\\cosmicd__annulet_of_absorption.wav");
		AddToOnBegin([]() { GetMusic().Play(); });
		AddToSlowUpdate([]() {GetMusic().UpdatePlayBuffer(); });
		Assert::AreEqual(Run(), 0);
	}

	TEST_METHOD(I_SameSourceMultiSoundMashUpTest)
	{
		SoftReset();
		SetWindowTitle("I_SameSourceMultiSoundMashUpTest");
		static auto speaker = AddSpeaker();
		static auto MyWindSound = AddSoundEffect("..\\..\\AAUnitTest\\res\\wind.ogg");
		static auto MyEnchantSound = ShortSound::AddShortSound("..\\..\\AAUnitTest\\res\\enchant.ogg");
		AddToKeyHandling([](KeyboardInput& kb) {
			if (kb.mouseButton1)
				GetSpeaker(speaker).PlayNoOverlap(MyEnchantSound);

			if (kb.mouseButton2)
				GetSpeaker(speaker).PlayInterrupt(MyEnchantSound);

			if (kb.mouseButton3)
				GetSpeaker(speaker).Play(MyWindSound);
			});
		Assert::AreEqual(Run(), 0);
	}
};
}
