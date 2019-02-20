#pragma once
#include <vector>

#include "displayManager/Display.h"
#include "camera/Camera.h"
#include "displayManager/Display.h"
#include "shaders/Shader.h"
#include "lighting/Lighting.h"
#include "util/TextureBank.h"
#include "models/Entity.h"
#include "models/Gravity.h"

extern Display display;
extern Shader shader;
extern Camera camera;
extern Lighting lighting;
extern TextureBank texBank;
extern std::vector<Entity> entities;
extern Gravity gravity;