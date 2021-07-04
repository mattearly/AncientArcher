#pragma once
#include "../../include/AncientArcher/Types.h"
#include "../Scene/Prop.h"
#include "MeshDrawInfo.h"
#include "TextureInfo.h"

#include <assimp/scene.h>

#include <vector>
#include <forward_list>

namespace AA {
namespace TextureLoader {
extern std::forward_list<TextureInfo> mLoadedTextures;
}
namespace ModelLoader {
void UnloadGameObject(const std::vector<MeshDrawInfo>& toUnload);
void processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans, Prop& out_model);
void processNode(aiNode* node, const aiScene* scene, Prop& out_model);
int LoadGameObjectFromFile(string path, Prop& out_model, bool animate = false);
} // end namespace ModelLoader
} // end namespace AA