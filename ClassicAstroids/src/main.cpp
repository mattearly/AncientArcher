#include <Loop.h>
#include <Lights.h>
#include <rand/rand.h>
#include <CollisionHandler.h>
#include "../include/Astroid.h"
#include "../include/DestroyAstroid.h"
#include "../include/HitAstroid.h"
#include "../include/SplitAstroid.h"

int main()
{
	static int cam_1 = LOOP->addCamera();
	LOOP->getCamera(cam_1).shiftYawAndPitch(0.f, -90.f); // look down

	DISPLAY->setWindowSize(800, 800, true);

	static int unlit_shader = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");

	static int lit_shader = LOOP->addShader("../assets/shaders/combinedLight.vert", "../assets/shaders/combinedLight.frag");
	// directional light for lit shader
	static AA::DirectionalLight directional_light;
	directional_light.Direction = glm::vec3(-0.45f, -1.f, 0.f);
	directional_light.Ambient = glm::vec3(0.05f);
	directional_light.Diffuse = glm::vec3(0.3f);
	directional_light.Specular = glm::vec3(1);
	setDirectionalLight(directional_light, LOOP->getShader(lit_shader));

	// BACKDROP
	static int starplane_object = LOOP->addObject("../assets/models/obj/starplane.obj", cam_1, lit_shader);
	LOOP->getGameObject(starplane_object).translateTo(glm::vec3(0, -99, 0));
	LOOP->getGameObject(starplane_object).scaleTo(glm::vec3(10, 1, 10));

	// PLAYER SHIP
	static int player_ship_object = LOOP->addObject("../assets/models/obj/flyingV.obj", cam_1, unlit_shader);
	LOOP->getGameObject(player_ship_object).translateTo(glm::vec3(0, -20, 0));
	LOOP->getGameObject(player_ship_object).scaleTo(glm::vec3(.6667f));
	LOOP->getGameObject(player_ship_object).rotateTo(glm::vec3(0, glm::radians(180.f), 0));

	// LAZER BULLET
	static int bullet_object = LOOP->addObject("../assets/models/obj/lazer.obj", cam_1, unlit_shader);
	LOOP->getGameObject(bullet_object).translateTo(LOOP->getGameObject(player_ship_object).getLocation());
	LOOP->getGameObject(bullet_object).scaleTo(glm::vec3(.3333f));
	LOOP->getGameObject(bullet_object).rotateTo(glm::vec3(0, glm::radians(180.f), 0));
	LOOP->getGameObject(bullet_object).setColliderSphere(LOOP->getGameObject(player_ship_object).getLocation(), .02f);

	static bool turnleft(false), turnright(false), moveforward(false), fireweap(false);
	auto hotkeys = [](AA::KeyboardInput& keypress)
	{
		if (keypress.w || keypress.upArrow)  // move in pointing direction
		{
			moveforward = true;
		}
		else if (!(keypress.w || keypress.upArrow))
		{
			moveforward = false;
		}

		if (keypress.a || keypress.leftArrow)  // turn left
		{
			turnleft = true;
		}
		else if (!(keypress.a || keypress.leftArrow))
		{
			turnleft = false;
		}

		if (keypress.d || keypress.rightArrow)  // turn right
		{
			turnright = true;
		}
		else if (!(keypress.d || keypress.rightArrow))
		{
			turnright = false;
		}

		if (keypress.spacebar || keypress.mouseButton1)
		{
			fireweap = true;
		}
		else if (!(keypress.spacebar || keypress.mouseButton1))
		{
			fireweap = false;
		}
	};
	LOOP->addToKeyHandling(hotkeys);

	static const float BOUNDRYSIZE = 11.1f;

	const float TURNSPEED = 210.f;
	static const float TURNSPEEDr = glm::radians(TURNSPEED);
	static const float MOVESPEED = 6.9f;
	static const float BULLETCOOLDOWN = .4187f;
	static const float BULLETSPEED = 21.f;

	static float xShipDir;
	static float zShipDir;
	static float xCamPos;
	static float zCamPos;
	static float xFireDir;
	static float zFireDir;

	static bool bulletOut(false);
	static bool bulletHitSomething(false);

	static bool bulletOnCooldown(false);
	static float bulletCooldownLength = 0.f;

	static auto resetBullet = []() {
		// reset variables
		bulletOut = false; bulletHitSomething = false;
		// reset location
		LOOP->getGameObject(bullet_object).translateTo(LOOP->getGameObject(player_ship_object).getLocation());
		LOOP->getGameObject(bullet_object).rotateTo(LOOP->getGameObject(player_ship_object).getRotation());
	};

	auto controlShip = [](float dt) {
		if (moveforward)
		{
			glm::vec3 dir = LOOP->getGameObject(player_ship_object).getRotation();
			xShipDir = sin(dir.y);
			zShipDir = cos(dir.y);
			LOOP->getGameObject(player_ship_object).advanceTranslate(glm::vec3(xShipDir, 0, zShipDir) * dt * MOVESPEED);

			//positive bounds
			if (LOOP->getGameObject(player_ship_object).getLocation().x > BOUNDRYSIZE)
				LOOP->getGameObject(player_ship_object).advanceTranslate(glm::vec3(-BOUNDRYSIZE * 2, 0, 0));

			if (LOOP->getGameObject(player_ship_object).getLocation().z > BOUNDRYSIZE)
				LOOP->getGameObject(player_ship_object).advanceTranslate(glm::vec3(0, 0, -BOUNDRYSIZE * 2));

			//negative bounds
			if (LOOP->getGameObject(player_ship_object).getLocation().x < -BOUNDRYSIZE)
				LOOP->getGameObject(player_ship_object).advanceTranslate(glm::vec3(BOUNDRYSIZE * 2, 0, 0));

			if (LOOP->getGameObject(player_ship_object).getLocation().z < -BOUNDRYSIZE)
				LOOP->getGameObject(player_ship_object).advanceTranslate(glm::vec3(0, 0, BOUNDRYSIZE * 2));

			if (!bulletOut)
			{
				//apply loc to bullet if it is not out
				LOOP->getGameObject(bullet_object).translateTo(LOOP->getGameObject(player_ship_object).getLocation());
			}

		}

		if (turnleft)
		{
			LOOP->getGameObject(player_ship_object).advanceRotation(glm::vec3(0, TURNSPEEDr * dt, 0));
			if (!bulletOut)
			{
				//apply rotation to bullet if it is not out
				LOOP->getGameObject(bullet_object).rotateTo(LOOP->getGameObject(player_ship_object).getRotation());
			}

		}

		if (turnright)
		{
			LOOP->getGameObject(player_ship_object).advanceRotation(glm::vec3(0, -TURNSPEEDr * dt, 0));
			if (!bulletOut)
			{
				//apply rotation to bullet if it is not out
				LOOP->getGameObject(bullet_object).rotateTo(LOOP->getGameObject(player_ship_object).getRotation());
			}

		}

		// bullet logic
		if (fireweap && !bulletOnCooldown)
		{
			bulletOut = true;
			bulletOnCooldown = true;
			glm::vec3 dir = LOOP->getGameObject(player_ship_object).getRotation();
			xShipDir = sin(dir.y);
			zShipDir = cos(dir.y);
			xFireDir = xShipDir;
			zFireDir = zShipDir;
		}

		if (bulletOnCooldown)
		{
			bulletCooldownLength += dt;

			if (bulletCooldownLength > BULLETCOOLDOWN)
			{
				bulletOnCooldown = false;
				bulletCooldownLength = 0.f;
				if (bulletOut)  //otherwise it is already reset
				{
					resetBullet();
				}
			}

			if (bulletOut)
			{
				if (!bulletHitSomething)
				{
					LOOP->getGameObject(bullet_object).advanceTranslate(glm::vec3(xFireDir, 0, zFireDir) * dt * BULLETSPEED);

					//positive bounds
					if (LOOP->getGameObject(bullet_object).getLocation().x > BOUNDRYSIZE)
						LOOP->getGameObject(bullet_object).advanceTranslate(glm::vec3(-BOUNDRYSIZE * 2, 0, 0));

					if (LOOP->getGameObject(bullet_object).getLocation().z > BOUNDRYSIZE)
						LOOP->getGameObject(bullet_object).advanceTranslate(glm::vec3(0, 0, -BOUNDRYSIZE * 2));

					//negative bounds
					if (LOOP->getGameObject(bullet_object).getLocation().x < -BOUNDRYSIZE)
						LOOP->getGameObject(bullet_object).advanceTranslate(glm::vec3(BOUNDRYSIZE * 2, 0, 0));

					if (LOOP->getGameObject(bullet_object).getLocation().z < -BOUNDRYSIZE)
						LOOP->getGameObject(bullet_object).advanceTranslate(glm::vec3(0, 0, BOUNDRYSIZE * 2));
				}
			}

		}
	};
	LOOP->addToDeltaUpdate(controlShip);

	// ASTROIDS
	static int go_asteroid = LOOP->addObject("../assets/models/obj/asteroid.obj", cam_1, unlit_shader);
	LOOP->getGameObject(go_asteroid).translateTo(glm::vec3(0, -20, -5));
	LOOP->getGameObject(go_asteroid).setColliderSphere(glm::vec3(0, -20, -5), 1.f);

	static int go_asteroid2 = LOOP->addObject("../assets/models/obj/asteroid2.obj", cam_1, unlit_shader);
	LOOP->getGameObject(go_asteroid2).translateTo(glm::vec3(-7, -20, -7));
	LOOP->getGameObject(go_asteroid2).setColliderSphere(glm::vec3(-7, -20, -7), 1.f);

	static std::vector<Astroid> astroids;
	astroids.push_back(createAstroid(go_asteroid, 0, false));
	astroids.push_back(createAstroid(go_asteroid2, 0, false));

	auto checkCollide = []()
	{
		if (bulletOut && !bulletHitSomething) {
			for (auto& ast : astroids)
			{
				if (AA::CollisionHandler::getInstance()->sphere_vs_Sphere_3D
				(
					LOOP->getGameObject(bullet_object).getColliderSphere(),
					LOOP->getGameObject(ast.object_id).getColliderSphere(ast.instance_id)
				))
				{
					resetBullet();
					hitAstroid(ast, astroids);
					return;
				}
			}
		}
	};
	LOOP->addToUpdate(checkCollide);

	static const float STARTDELAY = 3.f;
	auto moveasteroids = [](float dt) {
		static float live_timer = 0.f;
		live_timer += dt;
		if (live_timer < STARTDELAY)
			return;
		for (const auto& ast : astroids)
		{
			//positive bounds
			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).x > BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(-BOUNDRYSIZE * 2, 0, 0), ast.instance_id);

			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).z > BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(0, 0, -BOUNDRYSIZE * 2), ast.instance_id);

			//negative bounds
			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).x < -BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(BOUNDRYSIZE * 2, 0, 0), ast.instance_id);

			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).z < -BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(0, 0, BOUNDRYSIZE * 2), ast.instance_id);

			// standard movement
			LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(dt * ast.direction.x * ast.speed, 0, dt * ast.direction.y * ast.speed), ast.instance_id);
			LOOP->getGameObject(ast.object_id).advanceRotation(glm::vec3(dt * ast.rotation.x, dt * ast.rotation.y, dt * ast.rotation.z), ast.instance_id);
		}
	};
	LOOP->addToDeltaUpdate(moveasteroids);

	return LOOP->runMainLoop();
}