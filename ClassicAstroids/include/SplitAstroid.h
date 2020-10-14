#pragma once
#include "../../AAEngine/include/GameObject.h"
#include "../../AAEngine/include/Loop.h"
#include "Astroid.h"
#include <rand/rand.h>

/// <summary>
/// Turns a large (non-fragment) astroid into 4 small fragments.
/// </summary>
/// <param name="a">astroid that was hit</param>
/// <param name="astriods">ref to vector to add astroids to</param>
void splitAstroid(Astroid& a, std::vector<Astroid>& astriods)
{
	static const float SPLIT_SIZE = .40f;
	static const float RAND_DISTANCE = 1.f;

	AA::GameObject& our_ref = LOOP->getGameObject(a.object_id);
	glm::vec3 original_loc = our_ref.getLocation();

	// turn the orignal into a fragment
	our_ref.scaleTo(glm::vec3(SPLIT_SIZE));
	a.isFragment = true;

	// and make 3 more fragments
	for (int i = 0; i < 3; i++)
	{
		AA::InstanceDetails new_details(glm::vec3(SPLIT_SIZE), glm::vec3(0), original_loc + glm::vec3(NTKR(-RAND_DISTANCE, RAND_DISTANCE), 0, NTKR(-RAND_DISTANCE, RAND_DISTANCE)));
		new_details.mColliderSphere = new ColliderSphere(new_details.Translate, our_ref.getColliderSphere()->radius);
		our_ref.addInstance(new_details);
		astriods.push_back(createAstroid(a.object_id, i + 1, true));
	}
}