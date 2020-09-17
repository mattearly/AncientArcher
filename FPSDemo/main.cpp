#include <string>
#include <vector>
#include <Loop.h>
#include <memory>
#include <Skybox.h>
#include <iostream>
static constexpr float DEFAULTMOVESPEED = 10.f;
static constexpr float MAXSPEED = 400.f;
static constexpr float FLYINCR = 1.f;
int main(int argc, char* argv[])
{
	std::cout << "run: " << argv[0] << '\n';

	LOOP->setCursorToDisabled();
	LOOP->setToPerspectiveMouseHandling();
	static int mainCamId = LOOP->addCamera();
	static float currFlySpeed = DEFAULTMOVESPEED;
	static float prevFlySpeed = currFlySpeed;

	const auto wasd = [](AA::KeyboardInput& key) {
		static float frameCalculatedVelocity = 0.f;
		static glm::vec3 moveDir = glm::vec3(0.f);
		static glm::vec3 frontFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getFront());
		static glm::vec3 rightFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getRight());
		frameCalculatedVelocity = 0.0166f * currFlySpeed;

		if (key.w)
		{
			moveDir += frontFacingDir * frameCalculatedVelocity;
		}
		else if (key.s)
		{
			moveDir -= frontFacingDir * frameCalculatedVelocity;
		}
		if (key.a)
		{
			moveDir -= rightFacingDir * frameCalculatedVelocity;
		}
		else if (key.d)
		{
			moveDir += rightFacingDir * frameCalculatedVelocity;
		}
		LOOP->getCamera(mainCamId).shiftCurrentPosition(moveDir);
		moveDir = glm::vec3(0.f);
		frontFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getFront());
		rightFacingDir = glm::vec3(*LOOP->getCamera(mainCamId).getRight());
	};
	LOOP->addToKeyHandling(wasd);

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
		if (wheel.yOffset < -0.1f)
		{
			currFlySpeed -= FLYINCR;
			wheel.yOffset = 0;
		}
		// cap flyspeed
		if (currFlySpeed >= MAXSPEED)
		{
			currFlySpeed = MAXSPEED;
		}
		if (currFlySpeed <= 1.f)
		{
			currFlySpeed = 1.000001f;
		}
		if (currFlySpeed != prevFlySpeed)
		{
			prevFlySpeed = currFlySpeed;
		}
	};
	LOOP->addToScrollHandling(mousewheelflyspeed);

	// general asset locations (using trailing slashes)
	const std::string assetpath = "../assets/";
	const std::string skyboxfolder = "skyboxes/drakeq/";
	const std::string modelfolder = "models/dae/";

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

	// add a object
	static int cubeShaderId = LOOP->addShader((assetpath + "shaders/combinedLight.vert").c_str(), (assetpath + "shaders/combinedLight.frag").c_str());
	const std::string modelfileext = ".dae";
	static int cubeOneId = LOOP->addObject((assetpath + modelfolder + "check_cube" + modelfileext).c_str(), mainCamId, cubeShaderId);
	LOOP->getGameObject(cubeOneId).translateTo(glm::vec3(0.f, -3.f, -3.f));

	return LOOP->runMainLoop();
}
