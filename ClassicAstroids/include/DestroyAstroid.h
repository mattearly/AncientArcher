#pragma once
#include <vector>
#include "Astroid.h"

void DestroyAstroid(Astroid& a, std::vector<Astroid>& astroids)
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

	astroids.erase(astroids.begin() + isIt);
}
