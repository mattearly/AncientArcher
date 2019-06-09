#include "FirstPersonPlayer.h"
#include <Controls.h>
#include <glm/glm.hpp>
#include <iostream>

extern Controls g_controls;    // from display.cpp (engine)

void FirstPersonPlayer::render() const
{
  if (entity)
  {
    //draw
  }
}

/**
 * Adds a point light at the player location specified.
 */
void FirstPersonPlayer::addPointLight(glm::vec3 pos, Shader* shader)
{
  light->addPointLight(pos, shader);
}

/**
 * Moves the first point light to the newpos.
 */
void FirstPersonPlayer::movePlayerLight(glm::vec3 newpos, Shader* shader)
{
  light->movePointLight(0, newpos, shader);
}

void FirstPersonPlayer::increaseLegPower(float add) {
  if (legPower < 100.00f) {
    legPower += add;
    if (legPower > 100.00f) {
      legPower = 100.00f;
    }
  }
}

void FirstPersonPlayer::processControls()
{
  g_controls.firstPersonPlayerKeyboardInput(this);
}

float FirstPersonPlayer::getRunSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_SPEED;
}

float FirstPersonPlayer::getRisingSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_SPEED;
}

float FirstPersonPlayer::getJumpHeight() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_HEIGHT;
}

FirstPersonPlayer::FirstPersonPlayer() 
{
  legPower = 10.0f;
  light = new Lighting();
  jumpTimer = 0.0f;
}

FirstPersonPlayer::FirstPersonPlayer(float leg_power) 
{
  legPower = leg_power;
  light = new Lighting();
  jumpTimer = 0.0f;
}
