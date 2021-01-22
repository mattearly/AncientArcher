#include "../../include/Scene/WorldSettings.h"
namespace AA
{
WorldSettings::WorldSettings()
{
	mWorldUp = glm::vec3(0.f, 1.f, 0.f);
}
WorldSettings* WorldSettings::Get()
{
	static WorldSettings* world = new WorldSettings();
	return world;
}
}  // end namespace AA
