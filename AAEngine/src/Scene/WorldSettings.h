#pragma once
#include <glm\ext\vector_float3.hpp>
namespace AA
{
#define World WorldSettings::Get()

class WorldSettings
{
public:
  static WorldSettings* Get();
  glm::vec3 GetUpDir() { return mWorldUp; };
private:
  WorldSettings();
  glm::vec3 mWorldUp;
};
} // end namespace AA
