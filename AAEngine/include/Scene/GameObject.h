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
#pragma once
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Camera.h"
#include "../Renderer/OpenGL/OGLShader.h"
#include "InstanceDetails.h"
#include "../Renderer/MeshDrawInfo.h"
#include "ColliderSphere.h"
namespace AA
{
class GameObject
{
public:

	GameObject(const char* path, int camId, int shadId);
	GameObject(const char* path, int camId, int shadId, std::vector<InstanceDetails> details);

	// getters
	const glm::vec3& GetLocation() const;
	const glm::vec3& GetLocation(int which) const;
	const glm::vec3& GetRotation() const;
	const glm::vec3& GetRotation(int which) const;
	int GetModelMatrix(const int& which, glm::mat4& out_mat4) const;
	glm::mat4 GetModelMatrix(const int& which);
	const int GetShaderId() const noexcept;
	const int GetCameraId() const noexcept;
	const int GetObjectId() const noexcept;
	const std::size_t GetInstanceCount() const noexcept;
	const ColliderSphere* GetColliderSphere(uint32_t which = 0) const;
	bool IsSingleInstance() const;

	// setters
	void SetCamera(int id) noexcept;
	void SetShader(int id) noexcept;
	void SetColliderSphere(const glm::vec3& center, const float& radius, uint32_t which = 0, bool overwrite = true) noexcept;

	void SetScale(glm::vec3 amt, int which);
	void SetScale(glm::vec3 amt);
	void SetRotation(glm::vec3 new_rot, int which);
	void SetRotation(glm::vec3 new_rot);
	void SetTranslation(glm::vec3 amt, int which);
	void SetTranslation(glm::vec3 amt);

	void AddToScale(glm::vec3 amt);
	void AddToScale(glm::vec3 amt, int which);
	void AddToRotation(glm::vec3 radianAmt);
	void AddToRotation(glm::vec3 radianAmt, int which);
	void AddToTranslation(glm::vec3 amt);
	void AddToTranslation(glm::vec3 amt, int which);

	void AddInstance(const InstanceDetails& instance_details);
	bool RemoveInstance(int which);

	friend class AncientArcher;

private:

	// only AncientArcher can call draw on Objects
	void draw(const OGLShader& modelShader);

	int mCameraID = -1;
	int mShaderID = -1;
	int mObjectID = -1;

	std::vector<MeshDrawInfo> mMeshes;
	std::vector<InstanceDetails> mInstanceDetails;  // for instancing multiple objects, sized to the number of this object in our world

	// helpers
	void calculateNewModelMatrix(int which);
};
}  // end namespace AA
