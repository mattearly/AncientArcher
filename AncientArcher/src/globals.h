#pragma once

#include "camera/Camera.h"
#include "displayManager/Display.h"
#include "shaders/Shader.h"
#include "models/Collision.h"
#include "lighting/Lighting.h"
#include "util/TextureBank.h"
#include "util/DiffuseTexture.h"

extern Display display;
extern Shader shader;
extern Camera camera;
extern Lighting lighting;
extern Collision collision;
extern TextureBank texBank;
extern DiffuseTexture diffuseTex;

