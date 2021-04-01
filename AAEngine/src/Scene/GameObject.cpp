#include "../Scene/GameObject.h"
#include "../Renderer/OpenGL/OGLGraphics.h"
#include "../Scene/Camera.h"
#include "../Renderer/SceneLoader.h"
#include <iostream>

namespace AA
{
static int uniqueGameObjectIDs = 0;
//int GameObject::GetModelMatrix(const int& which, glm::mat4& out_mat4) const
//{
//	if (which < GetInstanceCount())
//	{
//		out_mat4 = mInstanceDetails.at(which).ModelMatrix;
//		return 0;
//	}
//	return -1;  // out of range
//}

//glm::mat4 GameObject::GetModelMatrix(const int& which)
//{
//	if (which < GetInstanceCount())
//		return mInstanceDetails.at(which).ModelMatrix;
//	else
//		throw("couldn't get model matrix");
//}

const int GameObject::GetCameraId() const noexcept
{
	return mCameraID;
}

const int GameObject::GetObjectId() const noexcept
{
	return mObjectID;
}

// returns the size of the InstanceDetails vector.
// a size of 1 would indicate a single instance of the object, accessable at location 0.
// a size of 0 should never be seen
//const std::size_t GameObject::GetInstanceCount() const noexcept
//{
//	return mInstanceDetails.size();
//}

/// <summary>
/// Returns the collider sphere (may be null) of the instance of the object (be sure to check for null to make sure it is set).
/// </summary>
/// <param name="which">The instance number to return the collider sphere of.</param>
/// <returns>Collider sphere of instance at (which) location.</returns>
//const ColliderSphere* GameObject::GetColliderSphere(uint32_t which) const
//{
//	if (which > mInstanceDetails.size())
//	{
//		throw("attempting to get a collider for an instance that does not exist");
//	}
//	return mInstanceDetails.at(which).mColliderSphere;  //todo: check there are enough instances if this has problems
//}

GameObject::GameObject(const char* path, int camId, bool lit)
{
	AA::SceneLoader::Get()->LoadGameObjectFromFile(mMeshes, path);
	//mInstanceDetails.push_back(InstanceDetails());
	mCameraID = camId;
	mIsLit = lit;
	mObjectID = uniqueGameObjectIDs++;
}

GameObject::GameObject(const char* path, int camId, bool lit, std::vector<InstanceDetails> details)
{
	AA::SceneLoader::Get()->LoadGameObjectFromFile(mMeshes, path);
	//mInstanceDetails = details;
	mCameraID = camId;
	mIsLit = lit;
	mObjectID = uniqueGameObjectIDs++;
}

void GameObject::SetCamera(int id) noexcept
{
	mCameraID = id;
}

//void GameObject::SetShader(int id) noexcept
//{
//	mShaderID = id;
//}
//
//void GameObject::SetColliderSphere(const glm::vec3& center, const float& radius, uint32_t which, bool overwrite) noexcept
//{
//	if (overwrite && mInstanceDetails.at(which).mColliderSphere != nullptr)  //todo: check there are enough instances if this has problems
//	{
//		delete mInstanceDetails.at(which).mColliderSphere;
//	}
//	mInstanceDetails.at(which).mColliderSphere = new ColliderSphere(center, radius);
//}
//
//void GameObject::AddInstance(const InstanceDetails& instance_details)
//{
//	mInstanceDetails.push_back(instance_details);
//}
//
void GameObject::draw()
{
	if (mCameraID == -1 || mObjectID == -1)
		throw("bad something in the draw");

	OGLGraphics::Render(mMeshes, finalModelMatrix, mIsLit);
}

void GameObject::updateFinalModelMatrix()
{
	finalModelMatrix = glm::mat4(1);
	finalModelMatrix = glm::scale(finalModelMatrix, scale);
	
	glm::quat rotation = rotationAngles;
	//glm::mat4 tmp_modelMatrix = glm::toMat4(rotation);
	finalModelMatrix = glm::toMat4(rotation) * finalModelMatrix;

	//const glm::vec3 rot_x = glm::vec3(1,0,0);
	//const glm::vec3 rot_y = glm::vec3(0,1,0);
	//const glm::vec3 rot_z = glm::vec3(0,0,1);
	//glm::quat tmp_quat1 = glm::rotate(rotation, rotationAngles.x, rot_x);
	//glm::quat tmp_quat2 = glm::rotate(rotation, rotationAngles.y, rot_y);
	//glm::quat tmp_quat3 = glm::rotate(rotation, rotationAngles.z, rot_z);
	//rotation = tmp_quat1 * tmp_quat2 * tmp_quat3;
	//rotation = glm::rotate(rotation, rotationAngles.y, rot_y);

	finalModelMatrix = glm::translate(finalModelMatrix, translation);	
}

//
//// --------------------------SCALE
//void GameObject::SetScale(glm::vec3 amt, int which)
//{
//	if (which < GetInstanceCount()) {
//		mInstanceDetails.at(which).Scale = amt;
//
//		// attempt to scale any collider spheres appropriately with the object
//		if (mInstanceDetails.at(which).mColliderSphere)  // take average of xyz and scale collider sphere radius by that
//		{
//			//todo : not do this or at least test it better
//			float avr = ((amt.x + amt.y + amt.z) / 3.f);
//			mInstanceDetails.at(which).mColliderSphere->radius = mInstanceDetails.at(which).mColliderSphere->radius * avr;
//		}
//		calculateNewModelMatrix(which);
//	}
//}
//
//void GameObject::SetScale(glm::vec3 amt)
//{
//	SetScale(amt, 0);
//}
//
//void GameObject::AddToScale(glm::vec3 amt)
//{
//	AddToScale(amt, 0);
//}
//
//void GameObject::AddToScale(glm::vec3 amt, int which)
//{
//	if (which < GetInstanceCount()) 
//	{
//		mInstanceDetails.at(which).Scale += amt;
//		calculateNewModelMatrix(which);
//	}
//}
//
//// ---------------------------ROTATE
//void GameObject::SetRotation(glm::vec3 new_rot)
//{
//	SetRotation(new_rot, 0);
//}
//
//void GameObject::SetRotation(glm::vec3 new_rot, int which)
//{
//	if (which < GetInstanceCount()) {
//		mInstanceDetails.at(which).Rotation = new_rot;
//	}
//	calculateNewModelMatrix(which);
//}
//
//void GameObject::AddToRotation(glm::vec3 radianAmt)
//{
//	AddToRotation(radianAmt, 0);
//}
//
//void GameObject::AddToRotation(glm::vec3 radianAmt, int which)
//{
//	if (which < GetInstanceCount()) 
//	{
//		mInstanceDetails.at(which).Rotation += radianAmt;
//		calculateNewModelMatrix(which);
//	}
//}
//
////-----------------------TRANSLATE
//void GameObject::AddToTranslation(glm::vec3 amt)
//{
//	AddToTranslation(amt, 0);
//}

//void GameObject::AddToTranslation(glm::vec3 amt, int which)
//{
//	if (which < GetInstanceCount()) {
//		mInstanceDetails.at(which).Translate += amt;
//		if (mInstanceDetails.at(which).mColliderSphere)
//		{
//			mInstanceDetails.at(which).mColliderSphere->center += amt;
//		}
//		calculateNewModelMatrix(which);
//	}
//}

//bool GameObject::RemoveInstance(int which)
//{
//	if (which > mInstanceDetails.size() - 1 || which < 0)
//	{
//		throw("invalid attempt to remove instance, size error");
//	}
//
//	mInstanceDetails.erase(mInstanceDetails.begin() + which);
//
//	return false;
//}
//
//void GameObject::SetTranslation(glm::vec3 to, int which)
//{
//	if (which < GetInstanceCount()) {
//		mInstanceDetails.at(which).Translate = to;
//		if (mInstanceDetails.at(which).mColliderSphere)
//		{
//			mInstanceDetails.at(which).mColliderSphere->center = to;
//		}
//	}
//	calculateNewModelMatrix(which);
//}

//void GameObject::SetTranslation(glm::vec3 to)
//{
//	SetTranslation(to, 0);
//}

/// <summary>
/// Checks to see if there is only a single instance of the game object.
/// </summary>
/// <returns>true if there is only one instance, false if there is more than one.</returns>
//bool GameObject::IsSingleInstance() const
//{
//	return mInstanceDetails.size() == 1;
//}

//const glm::vec3& GameObject::GetLocation() const
//{
//	return GetLocation(0);
//}

//const glm::vec3& GameObject::GetLocation(int which) const
//{
//	if (which < GetInstanceCount()) {
//		return mInstanceDetails.at(which).Translate;
//	}
//	else
//	{
//		throw("error getting translate, should never get here");
//	}
//}
//
//const glm::vec3& GameObject::GetRotation() const
//{
//	return GetRotation(0);
//}

//const glm::vec3& GameObject::GetRotation(int which) const
//{
//	return mInstanceDetails.at(which).Rotation;
//}

//void GameObject::calculateNewModelMatrix(int which)
//{
//	if (which < GetInstanceCount())
//	{
//		mInstanceDetails.at(which).calcModelMatrix();
//	}
//}
}  // end namespace AA