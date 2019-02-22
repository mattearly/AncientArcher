#pragma once
#include <vector>

#include "displayManager/Display.h"
#include "shaders/Shader.h"
#include "lighting/Lighting.h"
#include "util/TextureBank.h"
#include "models/Entity.h"

extern Display display;
extern Shader shader;
extern Lighting lighting;
extern TextureBank texBank;
extern std::vector<Entity> entities;

static constexpr const float GRAVITY = -9.81f;
static constexpr const float logic_checking_distance = 40.0f;
static constexpr const float world_height = 40.0f;
