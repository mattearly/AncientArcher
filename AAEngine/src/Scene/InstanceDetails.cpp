#include "../Scene/InstanceDetails.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
namespace AA
{

void InstanceDetails::calcModelMatrix()
{
  ModelMatrix = glm::mat4(1);

  // what most readings seem to recommend
  ModelMatrix = glm::scale(ModelMatrix, Scale);
  //ModelMatrix = glm::rotate(ModelMatrix, mRotateAngle, Rotation);

  static const glm::vec3 rot_ax_x(1, 0, 0);
  ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, rot_ax_x);
  static const glm::vec3 rot_ax_y(0, 1, 0);
  ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, rot_ax_y);
  static const glm::vec3 rot_ax_z(0, 0, 1);
  ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, rot_ax_z);

  ModelMatrix = glm::translate(ModelMatrix, Translate);

  // Order that does what we expect: Translate, Scale, Rotate [TSR]
  //ModelMatrix = glm::translate(ModelMatrix, Translate);
  //ModelMatrix = glm::scale(ModelMatrix, Scale);
  //static const glm::vec3 rot_ax_x(1, 0, 0);
  //static const glm::vec3 rot_ax_y(0, 1, 0);
  //static const glm::vec3 rot_ax_z(0, 0, 1);
  //ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, rot_ax_x);
  //ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, rot_ax_y);
  //ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, rot_ax_z);
}

InstanceDetails::InstanceDetails()
{
  Scale = glm::vec3(1);
  Translate = glm::vec3(0);
  Rotation = glm::vec3(0);
  ModelMatrix = glm::mat4(1);
  ColliderSphere* mColliderSphere = nullptr;
  calcModelMatrix();
}

InstanceDetails::InstanceDetails(glm::vec3 scale, glm::vec3 rot, glm::vec3 transl)
{
  Scale = scale;
  Translate = transl;
  Rotation = rot;
  calcModelMatrix();
}


}