#include <Loop.h>
#include <glm/glm.hpp>
int main()
{
	LOOP->setWindowTitle("keys: c-space-wasd-qe-zx-f1-f2");
	int cam = LOOP->addCamera();
	LOOP->setRenderDistance(cam, 2000.f);
	LOOP->getCamera(cam).shiftYawAndPitch(90.f, 0.f);
	int shad = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");

	// object 1, a boat without embedded textures
	static int boatfbxobj = LOOP->addObject("../assets/models/fbx/boat.fbx", cam, shad);
	LOOP->getGameObject(boatfbxobj).translateTo(glm::vec3(800, 0, -150));

	// object 2, a guy with embedded textures
	static int manfbxobj = LOOP->addObject("../assets/models/fbx/Ch18_nonPBR.fbx", cam, shad);
	LOOP->getGameObject(manfbxobj).translateTo(glm::vec3(800, -30, -550));

	static enum class CONTROL_WHICH { BOAT, MAN } control_which = CONTROL_WHICH::BOAT;

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

		if (keys.f1)
			control_which = CONTROL_WHICH::BOAT;
		if (keys.f2)
			control_which = CONTROL_WHICH::MAN;


	};
	LOOP->addToKeyHandling(moveboat);
	auto process_commands = [](float dt) {
		int obj_to_process;
		switch (control_which)
		{
		case CONTROL_WHICH::MAN:
			obj_to_process = manfbxobj;
			break;
		case CONTROL_WHICH::BOAT:
			obj_to_process = boatfbxobj;
			break;
		default:
			return;
		}


		if (moveaway)
			LOOP->getGameObject(obj_to_process).advanceTranslate(glm::vec3(400 * dt, 0, 0));
		if (movetowards)
			LOOP->getGameObject(obj_to_process).advanceTranslate(glm::vec3(-400 * dt, 0, 0));
		if (moveleft)
			LOOP->getGameObject(obj_to_process).advanceTranslate(glm::vec3(0, 0, -400 * dt));
		if (moveright)
			LOOP->getGameObject(obj_to_process).advanceTranslate(glm::vec3(0, 0, 400 * dt));
		if (movedown)
			LOOP->getGameObject(obj_to_process).advanceTranslate(glm::vec3(0, -200 * dt, 0));
		if (moveup)
			LOOP->getGameObject(obj_to_process).advanceTranslate(glm::vec3(0, 200 * dt, 0));
		if (rotatecc)
			LOOP->getGameObject(obj_to_process).advanceRotation(glm::vec3(glm::radians(-200 * dt), 0, 0));
		if (rotatec)
			LOOP->getGameObject(obj_to_process).advanceRotation(glm::vec3(glm::radians(200 * dt), 0, 0));
		if (spinleft)
			LOOP->getGameObject(obj_to_process).advanceRotation(glm::vec3(0, glm::radians(-200 * dt), 0));
		if (spinright)
			LOOP->getGameObject(obj_to_process).advanceRotation(glm::vec3(0, glm::radians(200 * dt), 0));
	};
	LOOP->addToDeltaUpdate(process_commands);

	return LOOP->runMainLoop();
}