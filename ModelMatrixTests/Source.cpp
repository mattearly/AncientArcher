#include <Loop.h>
int main()
{
	int mainCam = LOOP->addCamera();
	LOOP->setRenderDistance(mainCam, 2000.f);

	int unlitShader = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");

	static int obj1 = LOOP->addObject("../assets/models/fbx/Boat.FBX", mainCam, unlitShader);

	LOOP->getGameObject(obj1).translateTo(glm::vec3(0, -60, -300));

	LOOP->getGameObject(obj1).scaleTo(glm::vec3(.3f));

	auto rotateOnY = [](float dt) {
		LOOP->getGameObject(obj1).advanceRotation(glm::vec3(0, glm::radians(10.f) * dt, 0));
	};
	LOOP->addToDeltaUpdate(rotateOnY);

	auto rotateOnX = [](float dt) {
		LOOP->getGameObject(obj1).advanceRotation(glm::vec3(glm::radians(90.f) * dt, 0, 0));
	};
	LOOP->addToDeltaUpdate(rotateOnX);

	auto rotateOnZ = [](float dt) {
		LOOP->getGameObject(obj1).advanceRotation(glm::vec3(0, 0, glm::radians(5.f) * dt));
	};
	LOOP->addToDeltaUpdate(rotateOnZ);

	return LOOP->runMainLoop();
}