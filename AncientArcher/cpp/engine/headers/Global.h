#pragma once

#include <Display.h>
#include <Camera.h>
#include <Lighting.h>
#include <Entity.h>
#include <Controls.h>
#include <vector>

extern Display g_display;
extern Controls g_controls;
extern Camera g_camera;
extern Lighting g_lighting;
extern std::vector<Entity> g_entities;

static constexpr const float GRAVITY = -9.81f;                        // meters per second
static constexpr const float TERMINAL_VELOCITY = -53.00f;             // meters per second
static constexpr const float ENGINE_LOGIC_CHECKING_DISTANCE = 6.0f;
static constexpr const float MAX_WORLD_HEIGHT = 40.0f;

//static constexpr const float RENDER_DISTANCE = 50.f;
static constexpr const float RENDER_DISTANCE = 100.f;
