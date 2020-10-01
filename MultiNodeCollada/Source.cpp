#include <Loop.h>
#include <glm/glm.hpp>
#include <iostream>
int main()
{
	int cam = LOOP->addCamera();
	LOOP->setRenderDistance(cam, 2000.f);
	LOOP->getCamera(cam).shiftYawAndPitch(90.f, 0.f);
	int shad = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");
	static int lamp_post = LOOP->addObject("../assets/models/dae/lamp_post.dae", cam, shad);
	LOOP->getGameObject(lamp_post).translateTo(glm::vec3(13.2269, 0, -1.2905));

	static bool moveaway(false), movetowards(false), moveleft(false), moveright(false), moveup(false), movedown(false),
		rotatecc(false), rotatec(false), spinleft(false), spinright(false);
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

		if (keys.z)
			spinleft = true;
		else if (!keys.z)
			spinleft = false;

		if (keys.x)
			spinright = true;
		else if (!keys.x)
			spinright = false;

		if (keys.l) {
			glm::vec3 loc = LOOP->getGameObject(lamp_post).getLocation();
			std::cout << "sphere loc: " << loc.x << ", " << loc.y << ", " << loc.z << '\n';
		}

	};
	LOOP->addToKeyHandling(moveboat);
	static const float speed = 10;
	auto process = [](float dt) {
		if (moveaway)
			LOOP->getGameObject(lamp_post).advanceTranslate(glm::vec3(speed * dt, 0, 0));
		if (movetowards)
			LOOP->getGameObject(lamp_post).advanceTranslate(glm::vec3(-speed * dt, 0, 0));
		if (moveleft)
			LOOP->getGameObject(lamp_post).advanceTranslate(glm::vec3(0, 0, -speed * dt));
		if (moveright)
			LOOP->getGameObject(lamp_post).advanceTranslate(glm::vec3(0, 0, speed * dt));
		if (movedown)
			LOOP->getGameObject(lamp_post).advanceTranslate(glm::vec3(0, -speed * dt, 0));
		if (moveup)
			LOOP->getGameObject(lamp_post).advanceTranslate(glm::vec3(0, speed * dt, 0));
		if (rotatecc)
			LOOP->getGameObject(lamp_post).advanceRotation(glm::vec3(glm::radians(-speed * dt), 0, 0));
		if (rotatec)
			LOOP->getGameObject(lamp_post).advanceRotation(glm::vec3(glm::radians(speed * dt), 0, 0));
		if (spinleft)
			LOOP->getGameObject(lamp_post).advanceRotation(glm::vec3(0, glm::radians(-speed * dt), 0));
		if (spinright)
			LOOP->getGameObject(lamp_post).advanceRotation(glm::vec3(0, glm::radians(speed * dt), 0));
	};
	LOOP->addToDeltaUpdate(process);

	return LOOP->runMainLoop();
}