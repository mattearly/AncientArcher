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
  // reset
  finalModelMatrix = glm::mat4(1);
  finalModelMatrix = glm::translate(finalModelMatrix, translation);
  // apply current scale
  finalModelMatrix = glm::scale(finalModelMatrix, scale);

  // apply current rotation  (y only)
  if (eulerRotationY == 0.f)
    return;
  if (eulerRotationY < 0.f)
    eulerRotationY += 360.f;
  if (eulerRotationY > 360.f)
    eulerRotationY -= 360.f;
  finalModelMatrix = glm::rotate(finalModelMatrix, eulerRotationY, glm::vec3(0, 1, 0));

}

}  // end namespace AA