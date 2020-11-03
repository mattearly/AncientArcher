#pragma once
#include <vector>
#include "Astroid.h"
#include "../../AAEngine/include/Loop.h"

/// <summary>
/// fixes the instance ids of the rest of the astroids still left to match the rendering instance
/// </summary>
/// <param name="o_id">id of the one that is now gone</param>
/// <param name="i_id">id of the one that is now gone</param>
/// <param name="astroids">ref to our list of astroids</param>
void syncAstroids(int o_id, int i_id, std::vector<Astroid>& astroids)
{
	for (auto& ast : astroids)
	{
		if (ast.object_id == o_id && ast.instance_id > i_id)
		{
			ast.instance_id--;
		}
	}
}

void destroyAstroid(Astroid& a, std::vector<Astroid>& astroids)
{
	//debug boolean
	bool found = false;

	int isIt = 0;

	for (; isIt < astroids.size(); isIt++)
	{
		if (astroids[isIt].object_id == a.object_id && astroids[isIt].instance_id == a.instance_id)
		{
			found = true;
			break;
		}
	}

	// debug boolean check
	if (!found)
		throw("invalid destroy astroid call");

	int obj_id = a.object_id;
	int inst_id = a.instance_id;

	astroids.erase(astroids.begin() + isIt);

	LOOP->getGameObject(obj_id).removeInstance(inst_id);

	syncAstroids(obj_id, inst_id, astroids);
}
