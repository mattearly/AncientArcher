#include "AAGameObject.h"
#include "AAOGLGraphics.h"
#include "../cameraSys/AACamera.h"

#define assLoad AAOGLGraphics::getInstance()->loadGameObjectWithAssimp

static int uniqueIDs = 0;

int AAGameObject::getModelMatrix(const int& which, glm::mat4& out_mat4) const
{
  if (which < getInstanceCount()) 
  {
    out_mat4 = mInstanceDetails.at(which).ModelMatrix;
    return 0;
  }

  // out of range
  return -1;

}

const int AAGameObject::getShaderId() const noexcept
{
  return mShaderID;
}
const int AAGameObject::getCameraId() const noexcept
{
  return mCameraID;
}
const int AAGameObject::getObjectId() const noexcept
{
  return mObjectID;
}

// returns the size of the InstanceDetails vector. a size of 1 would indicate a single instance of the object, accessable at location 0. a size of 0 should never be seen
const std::size_t AAGameObject::getInstanceCount() const noexcept
{
  return mInstanceDetails.size();
}
AAGameObject::AAGameObject(const char* path, int camId, int shadId)
{
  //std::vector<MeshDrawInfo> meshes = assLoad(path, false);  // no triangulate
  assLoad(mMeshes, path, true);  // yes triangulate

  mInstanceDetails.push_back(InstanceDetails());
   
  //mMeshes = meshes;
  mCameraID = camId;
  mShaderID = shadId;
  mObjectID = uniqueIDs++;
}
AAGameObject::AAGameObject(const char* path, int camId, int shadId, std::vector<InstanceDetails> details)
{
  //std::vector<MeshDrawInfo> meshes = assLoad(path, false);  // no triangulate
  assLoad(mMeshes, path, true);  // yes triangulate

  mInstanceDetails = details;

  //mMeshes = meshes;
  mCameraID = camId;
  mShaderID = shadId;
  mObjectID = uniqueIDs++;

}
void AAGameObject::setCamera(int id) noexcept
{
  mCameraID = id;
}
void AAGameObject::setShader(int id) noexcept
{
  mShaderID = id;
}
void AAGameObject::draw(const AAOGLShader& modelShader)
{
  AAOGLGraphics::getInstance()->Render(mMeshes, mInstanceDetails, modelShader);
}
void AAGameObject::scale(glm::vec3 amt, int which)
{
  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).Scale = amt;
    updateModelMatrix(which);
  }
}
void AAGameObject::scale(glm::vec3 amt)
{
  scale(amt, 0);
}
void AAGameObject::rotate(float angle, glm::vec3 axis, int which)
{
  if (axis.x == 0.f && axis.y == 0.f && axis.z == 0.f)
  {
    //std::cout << "rotation axis setting can't be all 0's, unable to process rotate func\n";
    // rotation axis needs set
    return;
  }

  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).RotationAngle = angle;
    mInstanceDetails.at(which).RotationAxis = axis;
  }
  updateModelMatrix(which);

}
void AAGameObject::rotate(float radianAngle, glm::vec3 axis)
{
  rotate(radianAngle, axis, 0);
} 
void AAGameObject::translateTo(glm::vec3 to, int which)
{
  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).Translate = to;
  }
  updateModelMatrix(which);
}
void AAGameObject::translateTo(glm::vec3 to)
{
  translateTo(to, 0);
}
void AAGameObject::advanceScale(glm::vec3 amt)
{
  advanceScale(amt, 0);
}
void AAGameObject::advanceScale(glm::vec3 amt, int which)
{
  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).Scale += amt;  updateModelMatrix(which);

  }
}
void AAGameObject::advanceRotation(float angleAmtRadians)
{
  advanceRotation(angleAmtRadians, 0);
}
void AAGameObject::advanceRotation(float angleAmtRadians, int which)
{
  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).RotationAngle += angleAmtRadians;
    updateModelMatrix(which);
  }
}
void AAGameObject::advanceTranslate(glm::vec3 amt)
{
  advanceTranslate(amt, 0);
}
void AAGameObject::advanceTranslate(glm::vec3 amt, int which)
{
  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).Translate += amt;
    updateModelMatrix(which);
  }
}
void AAGameObject::changeRotateAxis(glm::vec3 axis)
{
  changeRotateAxis(axis, 0);
}
void AAGameObject::changeRotateAxis(glm::vec3 axis, int which)
{
  if (axis.x == 0.f && axis.y == 0.f && axis.z == 0.f)
  {
    // std::cout << "rotation axis setting can't be all 0's\n";
    // set a rotation angle axis needs set
    return;
  }

  if (which < getInstanceCount()) {
    mInstanceDetails.at(which).RotationAxis = axis;
    updateModelMatrix(which);
  }

}
bool AAGameObject::isSingleInstance() const
{
  return mInstanceDetails.size() == 1;
}
const glm::vec3& AAGameObject::getLocation() const
{
  return getLocation(0);
}
const glm::vec3& AAGameObject::getLocation(int which) const
{
  if (which < getInstanceCount()) {
    return mInstanceDetails.at(which).Translate;
  }

}
void AAGameObject::updateModelMatrix(int which)
{
  if (which < getInstanceCount())
  {
    mInstanceDetails.at(which).updateModelMatrix();
  }
}
//MeshDrawInfo::MeshDrawInfo(unsigned int a,/* unsigned int b, unsigned int e,*/ std::vector<TextureInfo> t, std::vector<unsigned int> el)
//  : vao(a),/* vbo(b), ebo(e),*/ textures(t), elements(el)
//{
//}
void InstanceDetails::updateModelMatrix()
{
  // reset ModelMatrix
  ModelMatrix = glm::mat4(1);

  // INTERNET SAYS: Scale, Rotate, Translate [SRT], however this does all kinds of weird sh*t in our case.
  //mModelMatrix = glm::scale(mModelMatrix, mScale);
  //mModelMatrix = glm::rotate(mModelMatrix, mRotateAngle, mRotate);
  //mModelMatrix = glm::translate(mModelMatrix, mTranslate);

  // update model matrix to it's current state
  // Order that does what we expect: Translate, Scale, Rotate [TSR]
  ModelMatrix = glm::translate(ModelMatrix, Translate);
  ModelMatrix = glm::scale(ModelMatrix, Scale);
  ModelMatrix = glm::rotate(ModelMatrix, RotationAngle, RotationAxis);

}
InstanceDetails::InstanceDetails()
{
  Scale = glm::vec3(1);
  RotationAxis = glm::vec3(1);
  Translate = glm::vec3(0);
  RotationAngle = 0.f;
  updateModelMatrix();
}
InstanceDetails::InstanceDetails(glm::vec3 scale, glm::vec3 rotAx, glm::vec3 transl, float rotAng)
{
  Scale = scale;

  // if invalid rotation axis entry
  if (rotAx.x == 0 && rotAx.y == 0 && rotAx.z == 0)
  {
    // set to all ones
    RotationAxis = glm::vec3(1);
  }
  else   //valid entry
  {
    RotationAxis = rotAx;
  }

  Translate = transl;
  RotationAngle = rotAng;

  updateModelMatrix();

}
