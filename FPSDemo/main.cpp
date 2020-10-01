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

	// general sample asset locations (using trailing slashes)
	const std::string assetpath = "..\\assets\\";

	// a single camera for perspective mode
	static int mainCamId = LOOP->addCamera();
	// up render distance so we can tell what is going on
	LOOP->setRenderDistance(mainCamId, 1000.f);

	// for our move direction and speed
	static float currFlySpeed = DEFAULTMOVESPEED;
	static float prevFlySpeed = currFlySpeed;
	static glm::vec3 moveDir = glm::vec3(0.f);
	static glm::vec3 frontFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getFront());
	static glm::vec3 rightFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getRight());

	// config engine loop for first person perspective view and controls
	LOOP->setCursorToDisabled();
	LOOP->setToPerspectiveMouseHandling();

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
		LOOP->getCamera(mainCamId).updateCameraVectors();
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

	// ----------------------- SKYBOX -------------------------------
	const std::string skyboxfolder = "skyboxes\\";
	const std::string order[6] = { "right", "left", "up", "down", "front", "back" };
	const std::string skyboxtype = "drakeq\\";
	const std::string skyboxfileext = ".png";
	std::vector<std::string> cubemapfiles;
	for (int j = 0; j < 6; ++j)
	{
		cubemapfiles.emplace_back(assetpath + skyboxfolder + skyboxtype + order[j] + skyboxfileext);
	}
	const std::shared_ptr<AA::Skybox> skybox = std::make_shared<AA::Skybox>(cubemapfiles);
	LOOP->setSkybox(skybox);

	//  ----------------------- LIT SHADER -------------------------------
	std::string vertShader = assetpath + "shaders\\combinedLight.vert";
	std::string fragShader = assetpath + "shaders\\combinedLight.frag";
	static int combinedLightId = LOOP->addShader(vertShader.c_str(), fragShader.c_str());

	//  ----------------------- COLLADA OBJECTS -------------------------------
	const std::string daemodelfolder = "models\\dae\\";
	const std::string daefileext = ".dae";
	static int cubeOneId = LOOP->addObject((assetpath + daemodelfolder + "check_cube" + daefileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(cubeOneId).translateTo(glm::vec3(0.f, -30.f, -30.f));
	// move our collada object
	const auto cubeMove = [](float dt) {
		LOOP->getGameObject(cubeOneId).advanceRotation(glm::vec3(glm::radians(dt * 15)));

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

	// add a grass plane collada object
	static int grassyOneId = LOOP->addObject((assetpath + daemodelfolder + "grassy1" + daefileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(grassyOneId).translateTo(glm::vec3(0.f, -100.f, 0.f));

	//  ----------------------- FBX OBJECTS -------------------------------
	const std::string fbxmodelfolder = "models\\fbx\\";
	const std::string fbxfileext = ".FBX";
	static int boatOneId = LOOP->addObject((assetpath + fbxmodelfolder + "Boat" + fbxfileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(boatOneId).translateTo(glm::vec3(0.f, -40.f, 30.f));
	LOOP->getGameObject(boatOneId).scaleTo(glm::vec3(.1f, .1f, .1f));
	// rotate our fbx object with delta time
	const auto boatSpin = [](float dt) {
		LOOP->getGameObject(boatOneId).advanceRotation(glm::vec3(0, glm::radians(dt * 10), 0));
	};
	LOOP->addToDeltaUpdate(boatSpin);

	//  ----------------------- WAVEFRONT (OBJ) OBJECTS -------------------------------
	const std::string objmodelfolder = "models\\obj\\";
	const std::string objfileext = ".obj";
	static int sphereOneId = LOOP->addObject((assetpath + objmodelfolder + "code_sphere" + objfileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(sphereOneId).translateTo(glm::vec3(0.f, -10.f, 10.f));

	static int weirdCubeId = LOOP->addObject((assetpath + objmodelfolder + "weird_cube" + objfileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(weirdCubeId).translateTo(glm::vec3(70.f, -45.f, 0.f));
	// move 'weirdcube' in a circle
	const auto cubeRingMov = [](float dt) {
		static float crmElapsedTime = 0.f;
		static float crmRadius = 1.f;
		LOOP->getGameObject(weirdCubeId).advanceTranslate(glm::vec3(crmRadius * cos(crmElapsedTime), 0, crmRadius * sin(crmElapsedTime)));
		crmElapsedTime += dt;
	};
	LOOP->addToDeltaUpdate(cubeRingMov);

	static int spaceshipId = LOOP->addObject((assetpath + objmodelfolder + "flyingV" + objfileext).c_str(), mainCamId, combinedLightId);
	LOOP->getGameObject(spaceshipId).translateTo(glm::vec3(0.f, 30.f, 0.f));

	// ----------------------- LIGHTS -----------------
	// point light
	static AA::PointLight pointLight;
	pointLight.Position = glm::vec3(0.f);
	pointLight.Ambient = glm::vec3(0.5f);
	pointLight.Diffuse = glm::vec3(0.5f);
	pointLight.Constant = 1.f;
	pointLight.Linear = .09f;
	pointLight.Quadratic = .032f;
	AA::NUM_POINT_LIGHTS++;
	setPointLight(pointLight, 0, LOOP->getShader(combinedLightId));
	// make point light stay on player (slowly)
	auto keepPointLightOnPlayer = []() {
		pointLight.Position = glm::vec3(
			LOOP->getCamera(mainCamId).getPosition()->x,
			LOOP->getCamera(mainCamId).getPosition()->y,
			LOOP->getCamera(mainCamId).getPosition()->z
		);
		setPointLight(pointLight, 0, LOOP->getShader(combinedLightId));
	};
	LOOP->addToSlowUpdate(keepPointLightOnPlayer);

	// directional light
	static AA::DirectionalLight dLight;
	dLight.Direction = glm::vec3(-0.45f, -1.f, 0.f);
	dLight.Ambient = glm::vec3(0.05f);
	dLight.Diffuse = glm::vec3(0.3f);
	dLight.Specular = glm::vec3(1);
	setDirectionalLight(dLight, LOOP->getShader(combinedLightId));

	// add a spot light
	static AA::SpotLight sptLight;
	sptLight.Position = glm::vec3(0, 0, 0);
	sptLight.Direction = glm::vec3(-1, 0, 0);
	sptLight.Ambient = glm::vec3(1);
	sptLight.Diffuse = glm::vec3(1);
	sptLight.Specular = glm::vec3(1);
	sptLight.Constant = 1.0f;
	sptLight.Linear = 0.09f;
	sptLight.Quadratic = .032f;
	//sptLight.CutOff = glm::cos(glm::radians(12.5f)); //orignal
	sptLight.CutOff = glm::cos(glm::radians(0.5f));
	//sptLight.OuterCutOff = glm::cos(glm::radians(20.5f)); //orignal
	sptLight.OuterCutOff = glm::cos(glm::radians(30.5f));
	AA::NUM_SPOT_LIGHTS++;
	setSpotLight(sptLight, 0, LOOP->getShader(combinedLightId));
	// make spot light stay on player cam like a flashlight (live)
	auto sudoHoldFlashlight = []() {
		sptLight.Position = *LOOP->getCamera(mainCamId).getPosition();
		sptLight.Direction = *LOOP->getCamera(mainCamId).getFront();
		setSpotLight(sptLight, 0, LOOP->getShader(combinedLightId));
	};
	LOOP->addToUpdate(sudoHoldFlashlight);

	// run the main engine now that it is set up
	return LOOP->runMainLoop();
}
