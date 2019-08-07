#pragma once

static constexpr const float GRAVITY = -9.81f;                        // meters per second
static constexpr const float TERMINAL_VELOCITY = -53.00f;             // meters per second
static constexpr const float ENGINE_LOGIC_CHECKING_DISTANCE = 6.0f;
static constexpr const float MAX_WORLD_HEIGHT = 40.0f;

//static constexpr const float RENDER_DISTANCE = 50.f;
static constexpr const float RENDER_DISTANCE = 100.f;

#include <glm/glm.hpp>
static constexpr const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
