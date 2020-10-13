#pragma once
#include "Astroid.h"
#include "DestroyAstroid.h"
#include "SplitAstroid.h"

// either destroys or splits
void hitAstroid(Astroid& a, std::vector<Astroid>& astroids)
{
	if (a.isFragment)
	{
		DestroyAstroid(a, astroids);
	}
	else
	{
		splitAstroid(a, astroids);
	}
}