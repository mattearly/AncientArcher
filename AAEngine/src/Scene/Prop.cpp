#include "Prop.h"
#include "Camera.h"
#include "../Renderer/OpenGL/OGLGraphics.h"
#include "../Renderer/ModelLoader.h"
#include <iostream>

namespace AA {
const int Prop::GetCamId() const noexcept {
  return mCameraID;
}

Prop::Prop(const char* path, int camId, SHADERTYPE shadertype) {
  ModelLoader::LoadGameObjectFromFile(*this, path);
  mCameraID = camId;
  mShaderType = shadertype;
}

void Prop::SetCamera(int id) noexcept {
  mCameraID = id;
}

void Prop::Draw() {
  if (mCameraID == -1)
    throw("camera not set for this prop");
  if (mModelMatrixChanged) {
    updateFinalModelMatrix();
    mModelMatrixChanged = false;
  }
  OGLGraphics::Render(mMeshes, currentWorldTransform, mShaderType);
}

void Prop::updateFinalModelMatrix() {
  // reset to identity matrix
  currentWorldTransform = glm::mat4(1);

  // apply translation
  currentWorldTransform = glm::translate(currentWorldTransform, translation);

  // fix angles between 0 & 360
  if (eulerRotationX < 0.f)
    eulerRotationX += 360.f;
  if (eulerRotationX > 360.f)
    eulerRotationX -= 360.f;

  if (eulerRotationY < 0.f)
    eulerRotationY += 360.f;
  if (eulerRotationY > 360.f)
    eulerRotationY -= 360.f;

  if (eulerRotationZ < 0.f)
    eulerRotationZ += 360.f;
  if (eulerRotationZ > 360.f)
    eulerRotationZ -= 360.f;

  currentWorldTransform = glm::rotate(currentWorldTransform, glm::radians(eulerRotationX), glm::vec3(1, 0, 0));
  currentWorldTransform = glm::rotate(currentWorldTransform, glm::radians(eulerRotationY), glm::vec3(0, 1, 0));
  currentWorldTransform = glm::rotate(currentWorldTransform, glm::radians(eulerRotationZ), glm::vec3(0, 0, 1));

  // apply current scale
  currentWorldTransform = glm::scale(currentWorldTransform, scale);
}

}  // end namespace AA