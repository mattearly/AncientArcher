#pragma once
#include "../../AAEngine/include/GameObject.h"
#include "../../AAEngine/include/Loop.h"
#include <rand/rand.h>

void splitAstroid(int game_object)
{
	static const float SPLIT_SIZE = .40f;
	static const float RAND_DISTANCE = 1.f;

	AA::GameObject& our_ref = LOOP->getGameObject(game_object);
	glm::vec3 original_loc = our_ref.getLocation();

	if (our_ref.isSingleInstance())
	{
		our_ref.scaleTo(glm::vec3(SPLIT_SIZE));
		for (int i = 0; i < 3; i++)
		{
			AA::InstanceDetails new_details(glm::vec3(SPLIT_SIZE), glm::vec3(0), original_loc + glm::vec3(NTKR(-RAND_DISTANCE, RAND_DISTANCE), 0, NTKR(-RAND_DISTANCE, RAND_DISTANCE)));
			new_details.mColliderSphere = new ColliderSphere(new_details.Translate, our_ref.getColliderSphere()->radius);
			our_ref.addInstance(new_details);
		}
	}
}