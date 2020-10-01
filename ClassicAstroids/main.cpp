#include <Loop.h>
#include <glm/glm.hpp>
#include <shaderSys\Lights.h>

int main()
{
	static int cam_1 = LOOP->addCamera();
	LOOP->getCamera(cam_1).shiftYawAndPitch(0.f, -90.f); // look down

	static int shader_unlit = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");

	static int shader_lit = LOOP->addShader("../assets/shaders/combinedLight.vert", "../assets/shaders/combinedLight.frag");
	// directional light for lit shader
	static AA::DirectionalLight dLight;
	dLight.Direction = glm::vec3(-0.45f, -1.f, 0.f);
	dLight.Ambient = glm::vec3(0.05f);
	dLight.Diffuse = glm::vec3(0.3f);
	dLight.Specular = glm::vec3(1);
	setDirectionalLight(dLight, LOOP->getShader(shader_lit));

	static int go_starplane = LOOP->addObject("../assets/models/obj/starplane.obj", cam_1, shader_lit);
	LOOP->getGameObject(go_starplane).translateTo(glm::vec3(0, -99, 0));
	LOOP->getGameObject(go_starplane).scaleTo(glm::vec3(10, 1, 10));


	static int go_ship = LOOP->addObject("../assets/models/obj/flyingV.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_ship).translateTo(glm::vec3(0, -20, 0));
	LOOP->getGameObject(go_ship).rotateTo(glm::vec3(0, glm::radians(180.f), 0));


	static bool turnleft(false), turnright(false), moveforward(false);
	auto hotkeys = [](AA::KeyboardInput& keyin)
	{
		if (keyin.w)
		{
			moveforward = true;
		}
		else if (!keyin.w)
		{
			moveforward = false;
		}

		if (keyin.a || keyin.leftArrow)  // turn left
		{
			turnleft = true;
		}
		else if (!(keyin.a || keyin.leftArrow))
		{
			turnleft = false;
		}

		if (keyin.d || keyin.rightArrow)  // turn right
		{
			turnright = true;
		}
		else if (!(keyin.d || keyin.rightArrow))
		{
			turnright = false;
		}
	};
	LOOP->addToKeyHandling(hotkeys);

	static float TURNSPEED = 70.f;
	static float MOVESPEED = 5.f;
	auto moveship = [](float dt) {
		if (moveforward)
		{
			LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(MOVESPEED, 0, MOVESPEED) * dt);
		}

		if (turnleft)
		{
			LOOP->getGameObject(go_ship).advanceRotation(glm::vec3(0, glm::radians(TURNSPEED) * dt, 0));
		}

		if (turnright)
		{
			LOOP->getGameObject(go_ship).advanceRotation(glm::vec3(0, -glm::radians(TURNSPEED) * dt, 0));
		}
	};
	LOOP->addToDeltaUpdate(moveship);

	return LOOP->runMainLoop();
}