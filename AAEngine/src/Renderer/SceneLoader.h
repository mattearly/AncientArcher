#pragma once
#include <assimp/scene.h>
#include <string>
#include "MeshDrawInfo.h"
#include <forward_list>
#include "TextureInfo.h"

namespace AA
{

class SceneLoader
{
public:
	static SceneLoader* Get();

	int LoadGameObjectFromFile(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path);

	void UnloadGameObject(const std::vector<MeshDrawInfo>& toUnload);

private:

	SceneLoader();

	void processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_);
	MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans);
	int loadMaterialTextures(const aiScene* scn, const aiMaterial* mat, aiTextureType type, std::string typeName, std::unordered_map<uint32_t, std::string>& out_texInfo);

	// holder vars
	std::string mModelDir = "";
	std::string mModelFileName = "";
	std::string mModelFileExtension = "";

	// Unordered Map of LoadedTextures
	std::forward_list<TextureInfo> mLoadedTextures;

	// helpers
	glm::mat4 aiMat4_to_glmMat4(const aiMatrix4x4& inMat);
	glm::vec3 aiVec3_to_glmVec3(const aiVector3D& inVec) noexcept;
	glm::vec4 aiColor4_to_glmVec4(const aiColor4D& inVec) noexcept;
	glm::quat aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept;

};
} // end namespace AA