#include <Loop.h>
#include <glm/glm.hpp>
int main()
{
	int cam = LOOP->addCamera();
	LOOP->setRenderDistance(cam, 2000.f);
	int shad = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");
	static int fbxobj = LOOP->addObject("../assets/models/fbx/boat.fbx", cam, shad);
	LOOP->getGameObject(fbxobj).translateTo(glm::vec3(800, 0, -150));
	LOOP->getGameObject(fbxobj).changeRotateAxis(glm::vec3(1, 0, 0));

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


	};
	LOOP->addToKeyHandling(moveboat);
	auto process = [](float dt) {
		if (moveaway)
			LOOP->getGameObject(fbxobj).advanceTranslate(glm::vec3(400 * dt, 0, 0));
		if (movetowards)
			LOOP->getGameObject(fbxobj).advanceTranslate(glm::vec3(-400 * dt, 0, 0));
		if (moveleft)
			LOOP->getGameObject(fbxobj).advanceTranslate(glm::vec3(0, 0, -400 * dt));
		if (moveright)
			LOOP->getGameObject(fbxobj).advanceTranslate(glm::vec3(0, 0, 400 * dt));
		if (movedown)
			LOOP->getGameObject(fbxobj).advanceTranslate(glm::vec3(0, -200 * dt, 0));
		if (moveup)
			LOOP->getGameObject(fbxobj).advanceTranslate(glm::vec3(0, 200 * dt, 0));
		if (rotatecc)
			LOOP->getGameObject(fbxobj).advanceRotation(glm::radians(-200 * dt));
		if (rotatec)
			LOOP->getGameObject(fbxobj).advanceRotation(glm::radians(200 * dt));
	};
	LOOP->addToDeltaUpdate(process);

	return LOOP->runMainLoop();
}