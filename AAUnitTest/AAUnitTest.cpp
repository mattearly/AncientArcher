#include "pch.h"
#include "CppUnitTest.h"
#include "CppUnitTestLogger.h"
#include <AncientArcher.h>
#include <Sound/ShortSound.h>
#include <Sound/SoundDevice.h>
#include <Utility/Files.h>
#include <Shader/DiffShader.h>
#include <Sound/LongSound.h>
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
		Engine->AddToDeltaUpdate([](float dt)
			{
				timeholder_A += dt;
				if (timeholder_A > 1.f)
				{
					Engine->Shutdown();
				}
			});
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(B_BaseReset)
	{
		Engine->SoftReset();
		static float timeholder_B = 0.f;
		Engine->AddToDeltaUpdate([](float dt) {
			timeholder_B += dt;
			if (timeholder_B > 1.f)
			{
				Engine->Shutdown();
			}
			});
		Engine->AddToSlowUpdate([]() {Engine->SetClearColor(glm::vec3(.4f, .4f, .4f)); });
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(C_AutoCamAndModelTest)
	{
		Engine->SoftReset();
		static int cam_C = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());
		int shader = Engine->AddShader(diff_vert_src, diff_frag_src);
		// local model, use your own
		static int dovecote = Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam_C, shader);
		Engine->GetGameObject(dovecote).SetTranslation(glm::vec3(0, 0, 0));
		Cam(cam_C).SetCurrentLocation(glm::vec3(0, 350, 2300));
		Cam(cam_C).SetMaxRenderDistance(2700.f);
		Engine->AddToDeltaUpdate([](float dt) {
			float Velocity = 1200;
			Cam(cam_C).ShiftCurrentLocation(glm::vec3(0, 0, -dt * Velocity));
			Obj(dovecote).AddToRotation(glm::vec3(0, -dt, 0));
			if (Cam(cam_C).GetLocation().z < 300)
			{
				Engine->Shutdown();
			}
			});
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(D_WASDMovementTest)
	{
		Engine->SoftReset();
		static int cam_D = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());
		int shader = Engine->AddShader(diff_vert_src, diff_frag_src);
		static int wasd = Engine->AddObject("..\\..\\AAUnitTest\\res\\wasd.obj", cam_D, shader);
		Engine->GetGameObject(wasd).SetScale(glm::vec3(3));
		Engine->GetGameObject(wasd).AddToRotation(glm::vec3(90, 0, 0));
		Cam(cam_D).SetCurrentLocation(glm::vec3(0, 0, 30));
		Cam(cam_D).SetCurrentPitch(0.f);
		Cam(cam_D).SetCurrentYaw(270.f);
		static bool left = false, right = false, forward = false, backwards = false;
		static bool pressedleft = false, pressedright = false, pressedforward = false, pressedbackwards = false;
		Engine->AddToKeyHandling([](KeyboardInput& kb) {
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
		Engine->AddToDeltaUpdate([](float dt) {
			if (forward)
				Cam(cam_D).ShiftCurrentLocation(glm::vec3(0, 0, -dt * VELOCITY));
			if (backwards)
				Cam(cam_D).ShiftCurrentLocation(glm::vec3(0, 0, dt * VELOCITY));
			if (left)
				Cam(cam_D).ShiftCurrentLocation(glm::vec3(-dt * VELOCITY, 0, 0));
			if (right)
				Cam(cam_D).ShiftCurrentLocation(glm::vec3(dt * VELOCITY, 0, 0));
			// rotate object
			Engine->GetGameObject(wasd).AddToRotation(glm::vec3(0, dt * .2, 0));
			});
		Engine->AddToSlowUpdate([]() {
			if (pressedleft && pressedright && pressedforward && pressedbackwards)
			{
				Engine->Shutdown();
			}
			});
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(E_FirstPersonPerspectiveTest)
	{
		Engine->SoftReset();
		static int cam_E = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());
		Cam(cam_E).SetCurrentLocation(glm::vec3(0, 0, 0));
		Cam(cam_E).ShiftYawAndPitch(90, 0);
		Cam(cam_E).SetMaxRenderDistance(3000);
		int shader = Engine->AddShader(diff_vert_src, diff_frag_src);
		static int fpp_test_text = Engine->AddObject("..\\..\\AAUnitTest\\res\\fpp_test_text.obj", cam_E, shader);
		Engine->GetGameObject(fpp_test_text).SetTranslation(glm::vec3(150, 0, -75));
		Engine->GetGameObject(fpp_test_text).SetScale(glm::vec3(10));
		Engine->GetGameObject(fpp_test_text).SetRotation(glm::vec3(0, glm::radians(-90.f), 0));
		static int negZ = Engine->AddObject("..\\..\\AAUnitTest\\res\\-z.obj", cam_E, shader);
		Obj(negZ).SetTranslation(glm::vec3(0, 75, -1500));
		Obj(negZ).SetScale(glm::vec3(50));
		static int posZ = Engine->AddObject("..\\..\\AAUnitTest\\res\\z.obj", cam_E, shader);
		Obj(posZ).SetTranslation(glm::vec3(0, 75, 1500));
		Obj(posZ).SetScale(glm::vec3(50));
		static int negX = Engine->AddObject("..\\..\\AAUnitTest\\res\\-x.obj", cam_E, shader);
		Obj(negX).SetTranslation(glm::vec3(-1500, 75, 0));
		Obj(negX).SetScale(glm::vec3(50));
		static int posX = Engine->AddObject("..\\..\\AAUnitTest\\res\\x.obj", cam_E, shader);
		Obj(posX).SetTranslation(glm::vec3(1500, 75, 0));
		Obj(posX).SetScale(glm::vec3(50));
		// First Person Mouse
		Engine->SetCursorToDisabled();
		Engine->SetReadMouseCurorAsFPP();
		Engine->AddToMouseHandling([](MouseInput& cursor) {
			Cam(cam_E).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
			});
		static bool left = false, right = false, forward = false, backwards = false;
		Engine->AddToKeyHandling([](KeyboardInput& kb) {
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
		Engine->AddToTimedOutKeyHandling([](KeyboardInput& kb) -> bool {
			if (kb.f11)
			{
				//todo: toggle fullscreen
				return true;
			}
			if (kb.p)
			{
				//todo: set to perspective
				Cam(cam_E).SetToPerspective();
				return true;
			}
			if (kb.o)
			{
				//todo: set to ortho
				Cam(cam_E).__setToOrtho();
				return true;
			}
			return false;
			});
		static float VELOCITY = 2;
		Engine->AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *Cam(cam_E).GetFront();
			glm::vec3 right_vec = *Cam(cam_E).GetRight();
			if (forward)
				Cam(cam_E).ShiftCurrentLocation(front_vec * VELOCITY);
			if (backwards)
				Cam(cam_E).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);
			if (left)
				Cam(cam_E).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);
			if (right)
				Cam(cam_E).ShiftCurrentLocation(right_vec * VELOCITY * .7f);
			//rotate x's and z's
			Obj(negZ).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			Obj(posZ).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			Obj(negX).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			Obj(posX).AddToRotation(glm::vec3(0, dt * VELOCITY, 0));
			});
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(F_CustomShaderTest)
	{
		Engine->SoftReset();
		static int cam_F = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());
		int shader = Engine->AddShader(Files::ReadToString("..\\..\\AAUnitTest\\res\\basicvert.glsl").c_str(), Files::ReadToString("..\\..\\AAUnitTest\\res\\basicfrag.glsl").c_str());
		// put in your own model to test it out
		int dovecote = Engine->AddObject("E:\\storage\\Models\\dovecote.fbx", cam_F, shader);
		Engine->GetGameObject(dovecote).SetTranslation(glm::vec3(0, -200, 0));
		int ground = Engine->AddObject("..\\..\\AAUnitTest\\res\\cube_stretched.obj", cam_F, shader);
		Obj(ground).SetScale(glm::vec3(100, 1, 100));
		Obj(ground).SetTranslation(glm::vec3(0, -300, 0));
		Cam(cam_F).SetCurrentLocation(glm::vec3(0, 0, 1600));
		Cam(cam_F).SetMaxRenderDistance(2700.f);
		static bool up = false, down = false, left = false, right = false;
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
		Engine->AddToDeltaUpdate([](float dt) {
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
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(G_SkyboxTest)
	{
		Engine->SoftReset();
		static int cam_G = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());
		Cam(cam_G).SetCurrentLocation(glm::vec3(0, 0, 0));
		Cam(cam_G).ShiftYawAndPitch(90, 0);
		Cam(cam_G).SetMaxRenderDistance(3000);
		const std::string skyboxfolder = "..\\..\\AAUnitTest\\res\\skybox\\";
		const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
		const std::string skyboxfileext = ".jpg";
		std::vector<std::string> cubemapfiles;
		for (int j = 0; j < 6; ++j)
		{
			cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
		}
		const std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>(cubemapfiles);
		Engine->SetSkybox(skybox);
		int shader = Engine->AddShader(diff_vert_src, diff_frag_src);
		static int fpp_test_text = Engine->AddObject("..\\..\\AAUnitTest\\res\\fpp_test_text.obj", cam_G, shader);
		Engine->GetGameObject(fpp_test_text).SetTranslation(glm::vec3(150, 0, -75));
		Engine->GetGameObject(fpp_test_text).SetScale(glm::vec3(10));
		Engine->GetGameObject(fpp_test_text).SetRotation(glm::vec3(0, glm::radians(-90.f), 0));
		int plane_thing = Engine->AddObject("..\\..\\AAUnitTest\\res\\cube_stretched.obj", cam_G, shader);
		Obj(plane_thing).SetScale(glm::vec3(11.f, .2f, 11.f));
		Obj(plane_thing).SetTranslation(glm::vec3(0, -20, 0));
		// First Person Mouse
		Engine->SetCursorToDisabled();
		Engine->SetReadMouseCurorAsFPP();
		Engine->AddToMouseHandling([](MouseInput& cursor) {
			Cam(cam_G).ShiftYawAndPitch(cursor.xOffset, cursor.yOffset);
			});
		static bool left = false, right = false, forward = false, backwards = false;
		Engine->AddToKeyHandling([](KeyboardInput& kb) {
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
		Engine->AddToDeltaUpdate([](float dt) {
			glm::vec3 front_vec = *Cam(cam_G).GetFront();
			glm::vec3 right_vec = *Cam(cam_G).GetRight();
			if (forward)
				Cam(cam_G).ShiftCurrentLocation(front_vec * VELOCITY);
			if (backwards)
				Cam(cam_G).ShiftCurrentLocation(-front_vec * VELOCITY * .5f);
			if (left)
				Cam(cam_G).ShiftCurrentLocation(-right_vec * VELOCITY * .7f);
			if (right)
				Cam(cam_G).ShiftCurrentLocation(right_vec * VELOCITY * .7f);
			});
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(H_SoundTest)
	{
		Engine->SoftReset();
		SoundDevice::Init();
		ShortSound MySpeaker;
		auto MyWindSound = ShortSound::AddShortSound("..\\..\\AAUnitTest\\res\\wind.ogg");
		MySpeaker.Play(MyWindSound);
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(I_LongSoundTest)
	{
		Engine->SoftReset();
		SoundDevice::Init();
		// TownTheme is a larger file and thus not in project, replace with your own music file dir
		static LongSound MyBackgroundMusic("E:\\downloads\\TownTheme.wav");
		//todo: figure out why this crashes (happens if these tests are run in sequence, something to do with the buffers or init from prev tests)
		MyBackgroundMusic.SetVolume(.01f);
		Engine->AddToOnBegin([]() {MyBackgroundMusic.Play(); });
		Engine->AddToSlowUpdate([]() {MyBackgroundMusic.UpdatePlayBuffer(); });
		Assert::AreEqual(Engine->Run(), 0);
	}

	TEST_METHOD(J_SameSourceMultiSoundMashUpTest)
	{
		Engine->SoftReset();
		SoundDevice::Init();
		static ShortSound MySpeaker;
		static auto MyWindSound = ShortSound::AddShortSound("..\\..\\AAUnitTest\\res\\wind.ogg");
		static auto MyEnchantSound = ShortSound::AddShortSound("..\\..\\AAUnitTest\\res\\enchant.ogg");
		Engine->AddToKeyHandling([](KeyboardInput& kb){
			if (kb.mouseButton1)
				MySpeaker.PlayNoOverlap(MyEnchantSound);

			if (kb.mouseButton2)
				MySpeaker.PlayInterrupt(MyEnchantSound);

			if (kb.mouseButton3)
				MySpeaker.Play(MyWindSound);
		});		
		Assert::AreEqual(Engine->Run(), 0);
	}
};
}
