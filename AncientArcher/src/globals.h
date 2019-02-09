#pragma once
#include <vector>

#include "displayManager/Display.h"
#include "camera/Camera.h"
#include "displayManager/Display.h"
#include "shaders/Shader.h"
#include "lighting/Lighting.h"
#include "util/TextureBank.h"
#include "util/DiffuseTexture.h"
#include "models/Entity.h"

extern Display display;
extern Shader shader;
extern Camera camera;
extern Lighting lighting;
extern TextureBank texBank;
extern DiffuseTexture diffuseTex;
extern std::vector<Entity> entities;
