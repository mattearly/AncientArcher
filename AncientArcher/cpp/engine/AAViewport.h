#pragma once
#include <glm/glm.hpp>
#include <mearly\Shader.h>
#include <memory>

class AAViewport
{

public:

  static AAViewport* getInstance();
  const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);

  void setToPerspective();
  void setRenderDistance(float distance);
  void setCurrentPosition(glm::vec3 pos);
  void setCurrentPitch(float pitch);
  void setCurrentYaw(float yaw);
  void shiftCurrentPosition(glm::vec3 offset);
  void shiftYawAndPith(float yawOffset, float pitchOffset);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;
  const glm::vec3* getPosition() const;
  const glm::vec3* getFront() const;
  const glm::vec3* getRight() const;
  float getYaw() const;
  float getPitch() const;

  friend class AAGameObject;
  friend class AADisplay;

private:

  void updateCameraVectors();

  void updateViewMatrix();

  glm::vec3 mPosition = glm::vec3(0);
  float mFieldOfView = 60.f;
  float mYaw = 0.f;
  float mPitch = 0.f;
  glm::vec3 mFront = glm::vec3(0, 0, -1);
  glm::vec3 mRight = glm::vec3(1, 0, 0);
  glm::vec3 mUp = glm::vec3(0, 1, 0);

  float mRenderDistance = 100.f;

  std::shared_ptr<Shader> mTexShader = std::make_shared<Shader>(
    "../AncientArcher/shader/VertexShader.glsl",
    "../AncientArcher/shader/TexFragShader.glsl"
    );

  std::shared_ptr<Shader> mCellShader = std::make_shared<Shader>(
    "../AncientArcher/shader/VertexShader.glsl",
    "../AncientArcher/shader/CellFragShader.glsl"
    );

};

