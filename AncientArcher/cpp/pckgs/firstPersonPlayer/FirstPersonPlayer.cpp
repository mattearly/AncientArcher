#include "FirstPersonPlayer.h"
#include <Controls.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

extern Controls g_controls;    // from display.cpp (engine)


FirstPersonPlayer::FirstPersonPlayer()
{
  light = std::make_unique<Lighting>();
  legPower = 10.0f;
  jumpTimer = 0.0f;
}

FirstPersonPlayer::FirstPersonPlayer(float leg_power)
{
  legPower = leg_power;
  light = std::make_unique<Lighting>();
  jumpTimer = 0.0f;
}


void FirstPersonPlayer::render() const
{
  if (model)
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
  g_controls.fppKeyboardIn(this);
}

void FirstPersonPlayer::processMovement(float deltaTime)
{/*
    static const float SPEED = 7.f;
  static std::size_t numEntities = 0;
  static unsigned int j = 0;
  numEntities = playerModel->getEntites()->size();
  if (moves.forward || moves.backward)
  {
	if (moves.forward) {
		_direction = 0;
	}
	if (moves.backward) {
		_direction = 1;
	}

	for (j = 0; j < numEntities; ++j) {
		(playerModel->getEntityPtr() + j)->moveBy((playerModel->getEntityPtr() + j)->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction));
	}
	g_camera.Position.x += playerModel->getEntityPtr()->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction).x;
  }
  else
  {
	  for (j = 0; j < numEntities; ++j) {
		  if ((playerModel->getEntityPtr() + j)->kinematics->vel.x != 0.0f) {
			if ((playerModel->getEntityPtr() + j)->kinematics->vel.x > 0.0f) {
				(playerModel->getEntityPtr() + j)->moveBy((playerModel->getEntityPtr() + j)->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction));
				g_camera.Position.x += playerModel->getEntityPtr()->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction).x;
			}
			else {
				(playerModel->getEntityPtr() + j)->kinematics->vel.x = 0.0f;
			}
		  }
	  }
  }*/
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

