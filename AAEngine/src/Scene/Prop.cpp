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
  ModelLoader::LoadGameObjectFromFile(mMeshes, path);
  mCameraID = camId;
  mShaderType = shadertype;
}

void Prop::SetCamera(int id) noexcept {
  mCameraID = id;
}

void Prop::draw() {
  if (mCameraID == -1)
    throw("camera not set for this prop");
  if (mModelMatrixChanged) {
    updateFinalModelMatrix();
    mModelMatrixChanged = false;
  }
  OGLGraphics::Render(mMeshes, finalModelMatrix, mShaderType);
}

void Prop::updateFinalModelMatrix() {
  // reset to identity matrix
  finalModelMatrix = glm::mat4(1);

  // apply translation
  finalModelMatrix = glm::translate(finalModelMatrix, translation);

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


  finalModelMatrix = glm::rotate(finalModelMatrix, glm::radians(eulerRotationX), glm::vec3(1, 0, 0));
  finalModelMatrix = glm::rotate(finalModelMatrix, glm::radians(eulerRotationY), glm::vec3(0, 1, 0));
  finalModelMatrix = glm::rotate(finalModelMatrix, glm::radians(eulerRotationZ), glm::vec3(0, 0, 1));

  // apply current scale
  finalModelMatrix = glm::scale(finalModelMatrix, scale);
}

}  // end namespace AA