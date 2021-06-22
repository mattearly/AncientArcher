#pragma once
#include "../../include/AncientArcher/Types.h"
#include "MeshDrawInfo.h"
#include "TextureInfo.h"

#include <assimp/scene.h>

#include <vector>
#include <forward_list>

namespace AA {
namespace TextureLoader{
extern std::forward_list<TextureInfo> mLoadedTextures;
}
namespace ModelLoader {
void UnloadGameObject(const std::vector<MeshDrawInfo>& toUnload);
MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans);
void processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_MeshInfo);
int LoadGameObjectFromFile(std::vector<MeshDrawInfo>& out_MeshInfo, string path);
} // end namespace ModelLoader
} // end namespace AA