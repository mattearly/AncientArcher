#pragma once
#include <vector>

#include "Display.h"
#include "Shader.h"
#include "Lighting.h"
#include "TextureBank.h"
#include "Entity.h"

extern Display display;
extern Shader texBankShader;
extern Lighting lighting;
extern TextureBank texBank;
extern std::vector<Entity> entities; 

static constexpr const float GRAVITY = -9.81f;                        // meters per second
static constexpr const float TERMINAL_VELOCITY = -53.00f;             // meters per second
static constexpr const float ENGINE_LOGIC_CHECKING_DISTANCE = 40.0f;
static constexpr const float MAX_WORLD_HEIGHT = 40.0f;
