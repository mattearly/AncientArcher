#include <string>
#include <vector>
#include <Loop.h>
#include <memory>
#include <math.h>
#include <Skybox.h>
#include <iostream>
#include <shaderSys\Lights.h>

// for our move speed controls
static constexpr float DEFAULTMOVESPEED = 10.f;
static constexpr float MAXSPEED = 400.f;
static constexpr float FLYINCR = 1.f;

int main(int argc, char* argv[])
{
	std::cout << "run: " << argv[0] << '\n';

	// config for first person perspective controls
	LOOP->setCursorToDisabled();
	LOOP->setToPerspectiveMouseHandling();

	// a single camera
	static int mainCamId = LOOP->addCamera();

	// for our move direction and speed
	static float currFlySpeed = DEFAULTMOVESPEED;
	static float prevFlySpeed = currFlySpeed;
	static glm::vec3 moveDir = glm::vec3(0.f);
	static glm::vec3 frontFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getFront());
	static glm::vec3 rightFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getRight());

	// add WASD key first person movement function
	const auto wasd = [](AA::KeyboardInput& key) {
		if (key.w)
		{
			moveDir += frontFacingDir;
		}
		else if (key.s)
		{
			moveDir -= frontFacingDir;
		}
		if (key.a)
		{
			moveDir -= rightFacingDir;
		}
		else if (key.d)
		{
			moveDir += rightFacingDir;
		}
	};
	LOOP->addToKeyHandling(wasd);

	const auto camMove = [](float dt) {
		static float frameCalculatedVelocity = 0.f;
		frameCalculatedVelocity = dt * currFlySpeed;
		LOOP->getCamera(mainCamId).shiftCurrentPosition(moveDir * frameCalculatedVelocity);
		moveDir = glm::vec3(0.f);
		frontFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getFront());
		rightFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getRight());
	};
	LOOP->addToDeltaUpdate(camMove);

	// add mouse movement to change our view direction
	const auto mouselook = [](AA::MouseInput& cursor)
	{
		LOOP->getCamera(mainCamId).shiftYawAndPitch(cursor.xOffset, cursor.yOffset);
	};
	LOOP->addToMouseHandling(mouselook);

	// add mouse scroll wheel to change fly speed
	const auto mousewheelflyspeed = [](AA::ScrollInput& wheel)
	{
		// set flyspeed when mouse wheel moves
		if (wheel.yOffset > 0.1f)
		{
			currFlySpeed += FLYINCR;
			wheel.yOffset = 0;
		}
		else if (wheel.yOffset < -0.1f)
		{
			currFlySpeed -= FLYINCR;
			wheel.yOffset = 0;
		}

		// cap flyspeed
		if (currFlySpeed >= MAXSPEED)
		{
			currFlySpeed = MAXSPEED;
		}
		else if (currFlySpeed <= 1.f)
		{
			currFlySpeed = 1.000001f;
		}

		// set flyspeed if it changed
		if (currFlySpeed != prevFlySpeed)
		{
			prevFlySpeed = currFlySpeed;
		}
	};
	LOOP->addToScrollHandling(mousewheelflyspeed);

	// general sample asset locations (using trailing slashes)
	const std::string assetpath = "..\\assets\\";
	const std::string skyboxfolder = "skyboxes\\drakeq\\";
	const std::string daemodelfolder = "models\\dae\\";
	const std::string fbxmodelfolder = "models\\fbx\\";
	const std::string gltfmodelfolder = "models\\glb\\";
	const std::string objmodelfolder = "models\\obj\\";

	// add a skybox
	const std::string order[6] = { "right", "left", "up", "down", "front", "back" };
	const std::string skyboxfileext = ".png";
	std::vector<std::string> cubemapfiles;
	for (int j = 0; j < 6; ++j)
	{
		cubemapfiles.emplace_back(assetpath + skyboxfolder + order[j] + skyboxfileext);
	}
	const std::shared_ptr<AA::Skybox> skybox = std::make_shared<AA::Skybox>(cubemapfiles);
	LOOP->setSkybox(skybox);

	// shader locations for models
	std::string vertShader = assetpath + "shaders\\combinedLight.vert";
	std::string fragShader = assetpath + "shaders\\combinedLight.frag";
	//std::string vertShader = "shaders/vert_default.glsl";
	//std::string fragShader = "shaders/frag_noLight.glsl";

	// load shader
	static int combinedLightId = LOOP->addShader(vertShader.c_str(), fragShader.c_str());

	// add a obj object
	const std::string objfileext = ".obj";
	static int sphereOneId = LOOP->addObject((assetpath + objmodelfolder + "code_sphere" + objfileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(sphereOneId).translateTo(glm::vec3(0.f, -10.f, 10.f));

	// add a collada object
	const std::string daefileext = ".dae";
	static int cubeOneId = LOOP->addObject((assetpath + daemodelfolder + "check_cube" + daefileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(cubeOneId).translateTo(glm::vec3(0.f, -30.f, -30.f));

	// move our collada object
	LOOP->getGameObject(cubeOneId).changeRotateAxis(glm::vec3(1, 1, 1));
	const auto cubeMove = [](float dt) {
		LOOP->getGameObject(cubeOneId).advanceRotation(glm::radians(dt * 5));

		static bool posDirection = true;
		if (LOOP->getGameObject(cubeOneId).getLocation().x > 100.f)
			posDirection = false;
		else if (LOOP->getGameObject(cubeOneId).getLocation().x < 0.f)
			posDirection = true;

		LOOP->getGameObject(cubeOneId).advanceTranslate(
			posDirection ?
			glm::vec3(dt * 5, 0, 0) :
			glm::vec3(-dt * 5, 0, 0)
		);
	};
	LOOP->addToDeltaUpdate(cubeMove);

	// add a fbx object
	const std::string fbxfileext = ".FBX";
	static int boatOneId = LOOP->addObject((assetpath + fbxmodelfolder + "Boat" + fbxfileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(boatOneId).translateTo(glm::vec3(0.f, -40.f, 30.f));
	LOOP->getGameObject(boatOneId).scaleTo(glm::vec3(.1f, .1f, .1f));

	// rotate our fbx object with delta time
	LOOP->getGameObject(boatOneId).changeRotateAxis(glm::vec3(0, 1, 0));
	const auto boatSpin = [](float dt) {
		LOOP->getGameObject(boatOneId).advanceRotation(glm::radians(dt * 10));
	};
	LOOP->addToDeltaUpdate(boatSpin);

	// add a gltf (.glb) object
	const std::string gltffileext = ".glb";
	static int weirdCubeId = LOOP->addObject((assetpath + gltfmodelfolder + "weird_cube" + gltffileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(weirdCubeId).translateTo(glm::vec3(70.f, -45.f, 0.f));

	// move in a circle
	LOOP->getGameObject(weirdCubeId).changeRotateAxis(glm::vec3(0, 1, 0));
	const auto cubeRingMov = [](float dt) {
		static float crmElapsedTime = 0.f;
		static float crmRadius = 1.f;
		LOOP->getGameObject(weirdCubeId).advanceTranslate(glm::vec3(
			crmRadius * cos(crmElapsedTime),
			0,
			crmRadius * sin(crmElapsedTime)
		)
		);
		crmElapsedTime += dt;
	};
	LOOP->addToDeltaUpdate(cubeRingMov);

	// up render distance so we can tell what is going on
	LOOP->setRenderDistance(mainCamId, 1000.f);

	//// add a point light
	//static AA::PointLight pointLight;
	//pointLight.Position = glm::vec4(0.f);
	//pointLight.Ambient = glm::vec4(0.5f);
	//pointLight.Diffuse = glm::vec4(0.5f);
	//pointLight.Constant = 1.f;
	//pointLight.Linear = .09f;
	//pointLight.Quadratic = .032f;
	//AA::NUM_POINT_LIGHTS++;
	//setPointLight(pointLight, 0, LOOP->getShader(combinedLightId));

	// add a directional light
	static AA::DirectionalLight dLight;
	dLight.Direction = glm::vec3(0.f, -1.f, 0.f);
	dLight.Ambient   = glm::vec3(0.2f);
	dLight.Diffuse   = glm::vec3(0.5f);
	dLight.Specular  = glm::vec3(1);
	setDirectionalLight(dLight, LOOP->getShader(combinedLightId));

	//// add a spot light
	//static AA::SpotLight sptLight;
	//sptLight.Position = glm::vec4(0, 0, 0, 0);
	//sptLight.Direction = glm::vec4(-1, 0, 0, 0);
	//sptLight.Ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.f);
	//sptLight.Diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.f);
	//sptLight.Specular = glm::vec4(1);
	//sptLight.Constant = 1.0f;
	//sptLight.Linear = 0.7f;
	//sptLight.Quadratic = .009f;
	//sptLight.CutOff = glm::cos(glm::radians(25.f));
	//sptLight.OuterCutOff = glm::cos(glm::radians(38.f));
	//AA::NUM_SPOT_LIGHTS++;
	//setSpotLight(sptLight, 0, LOOP->getShader(combinedLightId));

	return LOOP->runMainLoop();
}
