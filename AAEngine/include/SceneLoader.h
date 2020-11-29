#pragma once
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "MeshDrawInfo.h"
#include <unordered_map>

namespace AA
{
struct TextureInfo
{
	uint32_t accessId; // id to access it on the video mem (drawId)
	std::string type;
	std::string path;
};

class SceneLoader
{
public:
	static SceneLoader* getSceneLoader();

private:
	SceneLoader();

public:

	glm::mat4 aiMat4_to_glmMat4(const aiMatrix4x4& inMat);
	glm::vec3 aiVec3_to_glmVec3(const aiVector3D& inVec) noexcept;
	glm::vec4 aiColor4_to_glmVec4(const aiColor4D& inVec) noexcept;
	glm::quat aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept;

	int loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path);

private:

	void processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_);
	MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans);
	int loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::unordered_map<uint32_t, std::string>& out_texInfo);

	// holder vars
	std::string mLastDir = "";
	std::string mLastFileName = "";
	std::string mLastFileExtension = "";
	std::vector<TextureInfo> mTexturesLoaded;
};
} // end namespace AA