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
#include "GameObject.h"
#include "OGLGraphics.h"
#include "Camera.h"
#include "loader/SceneLoader.h"

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

	// out of range
	return -1;

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

// returns the size of the InstanceDetails vector. a size of 1 would indicate a single instance of the object, accessable at location 0. a size of 0 should never be seen
const std::size_t GameObject::getInstanceCount() const noexcept
{
	return mInstanceDetails.size();
}
GameObject::GameObject(const char* path, int camId, int shadId)
{
	//std::vector<MeshDrawInfo> meshes = assLoad(path, false);  // no triangulate
	//assLoad(mMeshes, path, true);  // yes triangulate

	SceneLoader sceneLoader;
	sceneLoader.loadGameObjectWithAssimp(mMeshes, path, true);

	mInstanceDetails.push_back(InstanceDetails());

	//mMeshes = meshes;
	mCameraID = camId;
	mShaderID = shadId;
	mObjectID = uniqueIDs++;
}

GameObject::GameObject(const char* path, int camId, int shadId, std::vector<InstanceDetails> details)
{
	//std::vector<MeshDrawInfo> meshes = assLoad(path, false);  // no triangulate
	//assLoad(mMeshes, path, true);  // yes triangulate

	SceneLoader sceneLoader;
	sceneLoader.loadGameObjectWithAssimp(mMeshes, path, true);

	mInstanceDetails = details;

	//mMeshes = meshes;
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
void GameObject::draw(const OGLShader& modelShader)
{
	OGLGraphics::getInstance()->Render(mMeshes, mInstanceDetails, modelShader);
}
void GameObject::scale(glm::vec3 amt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Scale = amt;
		updateModelMatrix(which);
	}
}
void GameObject::scale(glm::vec3 amt)
{
	scale(amt, 0);
}
void GameObject::rotate(float angle, glm::vec3 axis, int which)
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
void GameObject::rotate(float radianAngle, glm::vec3 axis)
{
	rotate(radianAngle, axis, 0);
}
void GameObject::translateTo(glm::vec3 to, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Translate = to;
	}
	updateModelMatrix(which);
}
void GameObject::translateTo(glm::vec3 to)
{
	translateTo(to, 0);
}
void GameObject::advanceScale(glm::vec3 amt)
{
	advanceScale(amt, 0);
}
void GameObject::advanceScale(glm::vec3 amt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Scale += amt;  updateModelMatrix(which);

	}
}
void GameObject::advanceRotation(float angleAmtRadians)
{
	advanceRotation(angleAmtRadians, 0);
}
void GameObject::advanceRotation(float angleAmtRadians, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).RotationAngle += angleAmtRadians;
		updateModelMatrix(which);
	}
}
void GameObject::advanceTranslate(glm::vec3 amt)
{
	advanceTranslate(amt, 0);
}
void GameObject::advanceTranslate(glm::vec3 amt, int which)
{
	if (which < getInstanceCount()) {
		mInstanceDetails.at(which).Translate += amt;
		updateModelMatrix(which);
	}
}
void GameObject::changeRotateAxis(glm::vec3 axis)
{
	changeRotateAxis(axis, 0);
}
void GameObject::changeRotateAxis(glm::vec3 axis, int which)
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
void GameObject::updateModelMatrix(int which)
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
}  // end namespace AA