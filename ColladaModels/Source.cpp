#include <Loop.h>
#include <glm/glm.hpp>
#include <iostream>
int main()
{
	int cam = LOOP->addCamera();
	LOOP->setRenderDistance(cam, 2000.f);
	int shad = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");
	static int __cube = LOOP->addObject("../assets/models/dae/check_cube.dae", cam, shad);
	LOOP->getGameObject(__cube).translateTo(glm::vec3(13.2269, 0, -1.2905));
	LOOP->getGameObject(__cube).changeRotateAxis(glm::vec3(1, 0, 0));

	static bool moveaway(false), movetowards(false), moveleft(false), moveright(false), moveup(false), movedown(false), rotatecc(false), rotatec(false);
	auto moveboat = [](AA::KeyboardInput& keys) {
		if (keys.w)
			moveaway = true;
		else if (!keys.w)
			moveaway = false;

		if (keys.s)
			movetowards = true;
		else if (!keys.s)
			movetowards = false;

		if (keys.a)
			moveleft = true;
		else if (!keys.a)
			moveleft = false;

		if (keys.d)
			moveright = true;
		else if (!keys.d)
			moveright = false;

		if (keys.spacebar)
			moveup = true;
		else if (!keys.spacebar)
			moveup = false;

		if (keys.c)
			movedown = true;
		else if (!keys.c)
			movedown = false;

		if (keys.q)
			rotatecc = true;
		else if (!keys.q)
			rotatecc = false;

		if (keys.e)
			rotatec = true;
		else if (!keys.e)
			rotatec = false;

		if (keys.l) {
			glm::vec3 loc = LOOP->getGameObject(__cube).getLocation();
			std::cout << "sphere loc: " << loc.x << ", " << loc.y << ", " << loc.z << '\n';
		}

	};
	LOOP->addToKeyHandling(moveboat);
	static const float speed = 50;
	auto process = [](float dt) {
		if (moveaway)
			LOOP->getGameObject(__cube).advanceTranslate(glm::vec3(speed * dt, 0, 0));
		if (movetowards)
			LOOP->getGameObject(__cube).advanceTranslate(glm::vec3(-speed * dt, 0, 0));
		if (moveleft)
			LOOP->getGameObject(__cube).advanceTranslate(glm::vec3(0, 0, -speed * dt));
		if (moveright)
			LOOP->getGameObject(__cube).advanceTranslate(glm::vec3(0, 0, speed * dt));
		if (movedown)
			LOOP->getGameObject(__cube).advanceTranslate(glm::vec3(0, -speed * dt, 0));
		if (moveup)
			LOOP->getGameObject(__cube).advanceTranslate(glm::vec3(0, speed * dt, 0));
		if (rotatecc)
			LOOP->getGameObject(__cube).advanceRotation(glm::radians(-speed * dt));
		if (rotatec)
			LOOP->getGameObject(__cube).advanceRotation(glm::radians(speed * dt));
	};
	LOOP->addToDeltaUpdate(process);

	return LOOP->runMainLoop();
}