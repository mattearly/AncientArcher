/*
GameObject
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
	copyright notice, this list of conditions and the
	following disclaimer.
* Redistributions in binary form must reproduce the above
	copyright notice, this list of conditions and the
	following disclaimer in the documentation and/or other
	materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
	contributors may be used to endorse or promote products
	derived from this software without specific prior
	written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "../include/GameObject.h"
#include "../include/OGLGraphics.h"
#include "../include/Camera.h"
#include "../include/SceneLoader.h"

namespace AA
{
static int uniqueIDs = 0;
int GameObject::getModelMatrix(const int& which, glm::mat4& out_mat4) const
{
	if (which < getInstanceCount())
	{
		out_mat4 = mInstanceDetails.at(which).ModelMatrix;
		return 0;
	}
	return -1;  // out of range
}

glm::mat4 GameObject::getModelMatrix(const int& which)
{
	if (which < getInstanceCount())
		return mInstanceDetails.at(which).ModelMatrix;
	else
		throw("couldn't get model matrix");
}

const int GameObject::getShaderId() const noexcept
{
	return mShaderID;
}

const int GameObject::getCameraId() const noexcept
{
	return mCameraID;
}

const int GameObject::getObjectId() const noexcept
{
	return mObjectID;
}

// returns the size of the InstanceDetails vector.
// a size of 1 would indicate a single instance of the object, accessable at location 0.
// a size of 0 should never be seen
const std::size_t GameObject::getInstanceCount() const noexcept
{
	return mInstanceDetails.size();
}

/// <summary>
/// Returns the collider sphere (may be null) of the instance of the object (be sure to check for null to make sure it is set).
/// </summary>
/// <param name="which">The instance number to return the collider sphere of.</param>
/// <returns>Collider sphere of instance at (which) location.</returns>
const ColliderSphere* GameObject::getColliderSphere(uint32_t which) const
{
	if (which > mInstanceDetails.size())
	{
		throw("attempting to get a collider for an instance that does not exist");
	}
	return mInstanceDetails.at(which).mColliderSphere;  //todo: check there are enough instances if this has problems
}

GameObject::GameObject(const char* path, int camId, int shadId)
{
	AA::SceneLoader::getSceneLoader()->loadGameObjectWithAssimp(mMeshes, path);
	mInstanceDetails.push_back(InstanceDetails());
	mCameraID = camId;
	mShaderID = shadId;
	mObjectID = uniqueIDs++;
}

GameObject::GameObject(const char* path, int camId, int shadId, std::vector<InstanceDetails> details)
{
	AA::SceneLoader::getSceneLoader()->loadGameObjectWithAssimp(mMeshes, path);
	mInstanceDetails = details;
	mCameraID = camId;
	mShaderID = shadId;
	mObjectID = uniqueIDs++;
}

void GameObject::setCamera(int id) noexcept
{
	mCameraID = id;
}

void GameObject::setShader(int id) noexcept
{
	mShaderID = id;
}

void GameObject::setColliderSphere(const glm::vec3& center, const float& radius, uint32_t which, bool overwrite) noexcept
{
	if (overwrite && mInstanceDetails.at(which).mColliderSphere != nullptr)  //todo: check there are enough instances if this has problems
	{
		delete mInstanceDetails.at(which).mColliderSphere;
	}
	mInstanceDetails.at(which).mColliderSphere = new ColliderSphere(center, radius);
}

void GameObject::addInstance(const InstanceDetails& instance_details)
{
	mInstanceDetails.push_back(instance_details);
}

void GameObject::draw(const OGLShader& modelShader)
{
	if (mInstanceDetails.size() > 0)
		OGLGraphics::getInstance()->Render(mMeshes, mInstanceDetails, modelShader);
}

// --------------------------SCALE
void GameObject::setScale(glm::vec3 amt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Scale = amt;

		// attempt to scale any collider spheres appropriately with the object
		if (mInstanceDetails.at(which).mColliderSphere)  // take average of xyz and scale collider sphere radius by that
		{
			//todo : not do this or at least test it better
			float avr = ((amt.x + amt.y + amt.z) / 3.f);
			mInstanceDetails.at(which).mColliderSphere->radius = mInstanceDetails.at(which).mColliderSphere->radius * avr;
		}
		updateModelMatrix(which);
	}
}

void GameObject::setScale(glm::vec3 amt)
{
	setScale(amt, 0);
}

void GameObject::advanceScale(glm::vec3 amt)
{
	advanceScale(amt, 0);
}

void GameObject::advanceScale(glm::vec3 amt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Scale += amt;
		updateModelMatrix(which);
	}
}

// ---------------------------ROTATE
void GameObject::setRotation(glm::vec3 new_rot)
{
	setRotation(new_rot, 0);
}

void GameObject::setRotation(glm::vec3 new_rot, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Rotation = new_rot;
	}
	updateModelMatrix(which);
}

void GameObject::advanceRotation(glm::vec3 radianAmt)
{
	advanceRotation(radianAmt, 0);
}

void GameObject::advanceRotation(glm::vec3 radianAmt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Rotation += radianAmt;
		updateModelMatrix(which);
	}
}

//-----------------------TRANSLATE
void GameObject::advanceTranslate(glm::vec3 amt)
{
	advanceTranslate(amt, 0);
}

void GameObject::advanceTranslate(glm::vec3 amt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Translate += amt;
		if (mInstanceDetails.at(which).mColliderSphere)
		{
			mInstanceDetails.at(which).mColliderSphere->center += amt;
		}
		updateModelMatrix(which);
	}
}

bool GameObject::removeInstance(int which)
{
	if (which > mInstanceDetails.size() - 1 || which < 0)
	{
		throw("invalid attempt to remove instance, size error");
	}

	mInstanceDetails.erase(mInstanceDetails.begin() + which);

	return false;
}

void GameObject::setTranslation(glm::vec3 to, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Translate = to;
		if (mInstanceDetails.at(which).mColliderSphere)
		{
			mInstanceDetails.at(which).mColliderSphere->center = to;
		}
	}
	updateModelMatrix(which);
}

void GameObject::setTranslation(glm::vec3 to)
{
	setTranslation(to, 0);
}

/// <summary>
/// Checks to see if there is only a single instance of the game object.
/// </summary>
/// <returns>true if there is only one instance, false if there is more than one.</returns>
bool GameObject::isSingleInstance() const
{
	return mInstanceDetails.size() == 1;
}

const glm::vec3& GameObject::getLocation() const
{
	return getLocation(0);
}

const glm::vec3& GameObject::getLocation(int which) const
{
	if (which < getInstanceCount()) {
		return mInstanceDetails.at(which).Translate;
	}
	else
	{
		throw("error getting translate, should never get here");
	}
}

const glm::vec3& GameObject::getRotation() const
{
	return getRotation(0);
}

const glm::vec3& GameObject::getRotation(int which) const
{
	return mInstanceDetails.at(which).Rotation;
}

void GameObject::updateModelMatrix(int which)
{
	if (which < getInstanceCount())
	{
		mInstanceDetails.at(which).updateModelMatrix();
	}
}

void InstanceDetails::updateModelMatrix()
{
	ModelMatrix = glm::mat4(1);

	// what most readings seem to recommend, doesn't work right in our case [SRT]
	//ModelMatrix = glm::scale(mModelMatrix, mScale);
	//ModelMatrix = glm::rotate(mModelMatrix, mRotateAngle, mRotate);
	//ModelMatrix = glm::translate(mModelMatrix, mTranslate);

	// Order that does what we expect: Translate, Scale, Rotate [TSR]
	ModelMatrix = glm::translate(ModelMatrix, Translate);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
	static const glm::vec3 rot_ax_x(1, 0, 0);
	static const glm::vec3 rot_ax_y(0, 1, 0);
	static const glm::vec3 rot_ax_z(0, 0, 1);
	ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, rot_ax_x);
	ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, rot_ax_y);
	ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, rot_ax_z);
}

InstanceDetails::InstanceDetails()
{
	Scale = glm::vec3(1);
	Translate = glm::vec3(0);
	Rotation = glm::vec3(0);
	ModelMatrix = glm::mat4(1);
	ColliderSphere* mColliderSphere = nullptr;
	updateModelMatrix();
}

InstanceDetails::InstanceDetails(glm::vec3 scale, glm::vec3 rot, glm::vec3 transl)
{
	Scale = scale;
	Translate = transl;
	Rotation = rot;
	updateModelMatrix();
}
}  // end namespace AA