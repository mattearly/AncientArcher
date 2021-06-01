#pragma once
#include "../../include/AncientArcher/Types.h"
#include "../../include/AncientArcher/Utility/SceneCheck.h"
#include "../Scene/BoneInfo.h"
#include "../Utility/Conversions.h"
#include "../Settings/Settings.h"
#include "TextureInfo.h"
#include "TextureLoader.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <forward_list>
#include <map>

namespace AA {

extern string mModelDir;
extern string mModelFileName;
extern string mModelFileExtension;
extern std::forward_list<TextureInfo> mLoadedTextures;
extern int MAX_BONE_WEIGHTS;
extern int m_BoneCounter;

namespace ModelLoader {

inline void SetVertexBoneData(AnimVertex& vertex, int boneID, float weight) {
  for (int i = 0; i < MAX_BONE_WEIGHTS; ++i) {
    if (vertex.BoneIds[i] < 0) {
      vertex.Weights[i] = weight;
      vertex.BoneIds[i] = boneID;
    }
  }
}

inline void ExtractBoneWeightForVertices(std::vector<AnimVertex>& loaded_verts, const aiMesh* mesh, const aiScene* scene, std::map<string, BoneInfo>& out_BoneInfoMap) {
  for (u32 b = 0; b < mesh->mNumBones; ++b) {
    int boneID = -1;
    std::string boneName = mesh->mBones[b]->mName.C_Str();
    if (out_BoneInfoMap.find(boneName) == out_BoneInfoMap.end()) {  // end is for not found: i.e. bone not already in list 
      BoneInfo newBoneInfo{};
      newBoneInfo.id = m_BoneCounter;
      newBoneInfo.offset = aiMat4_to_glmMat4(mesh->mBones[b]->mOffsetMatrix);
      out_BoneInfoMap[boneName] = newBoneInfo;
      boneID = m_BoneCounter;
      m_BoneCounter++;
    } else {
      boneID = out_BoneInfoMap[boneName].id;
    }
    assert(boneID != -1);
    auto weights = mesh->mBones[b]->mWeights;
    int numWeights = mesh->mBones[b]->mNumWeights;

    for (int w = 0; w < numWeights; ++w) {
      int vertexId = weights[w].mVertexId;
      float weight = weights[w].mWeight;
      assert(vertexId <= loaded_verts.size());
      SetVertexBoneData(loaded_verts[vertexId], boneID, weight);
    }
  }
}

inline MeshDrawInfo processAnimMesh(
  const aiMesh* mesh, 
  const aiScene* scene, 
  aiMatrix4x4* trans, 
  std::map<string, BoneInfo>& out_BoneInfoMap) {

  if (!mesh->HasPositions())
    throw("no positions");
  if (mesh->mTextureCoords == nullptr)
    throw("no texture coords");
  if (!mesh->HasNormals())
    throw("no normals");

  u32 num_of_vertices_on_mesh = mesh->mNumVertices;

  std::vector<AnimVertex> loaded_verts;
  loaded_verts.reserve(num_of_vertices_on_mesh);

  for (u32 i = 0; i < num_of_vertices_on_mesh; ++i) {
    loaded_verts.push_back(AnimVertex(
      aiVec3_to_glmVec3(mesh->mVertices[i]),
      aiVec3_to_glmVec3(mesh->mNormals[i]),
      vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y),
      ivec4(-1),  //bone ids
      vec4(0.0f)  //weights
    ));
  }

  ExtractBoneWeightForVertices(loaded_verts, mesh, scene, out_BoneInfoMap);

  // get the indices to draw triangle faces with
  std::vector<u32> loaded_elements;
  for (u32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (u32 j = 0; j < face.mNumIndices; ++j) {
      loaded_elements.push_back(face.mIndices[j]);
    }
  }

  // get the materials
  const aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
  std::unordered_map<u32, std::string> all_loaded_textures;

  // get the albedo (diffuse) textures
  std::unordered_map<u32, std::string> albedo_textures;
  if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_DIFFUSE, "Albedo", albedo_textures) == 0) {
    for (auto& a_tex : albedo_textures) {
      all_loaded_textures.insert(all_loaded_textures.end(), a_tex);
      //std::cout << "found&loaded Albedo texture\n";
    }
  }

  /*
  int shading_model;
  ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_model);
  //std::cout << "shading model: " << shading_model << '\n';
  ai_real shininess = .1f;

  if (shading_model == aiShadingMode_Phong) {
    //std::cout << "shading model is phong\n";

    std::unordered_map<u32, std::string> specular_textures;
    if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_SPECULAR, "Specular", specular_textures) == 0) {
      for (auto& s_tex : specular_textures) {
        all_loaded_textures.insert(all_loaded_textures.end(), s_tex);
        //std::cout << "found&loaded Specular texture\n";
      }
    }

    if (!ai_material->Get(AI_MATKEY_SHININESS, shininess)) {
      // set shininess to a default if it failed
      shininess = 6.1f;
      //std::cout << "shininess not found, shininess defaulted to 6.1f\n";
      //assimpDoesntFindShininessLetsDoItInstead(path, &shininess);
    }
  }
  */

  //aiColor4D spec_color;
  //if (AI_SUCCESS != aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &spec_color))
  //{
  //	// set spec_color to a default if it failed to find
  //	spec_color = aiColor4D(0.1f, 0.1f, 0.1f, 1.f);
  //	std::cout << "specular not found, defaulted to .1f\n";
  //}

  u32 vao = 0;
  switch (Settings::Get()->GetOptions().renderer) {
  case RenderingFramework::OPENGL:
    vao = OGLGraphics::UploadAnimMesh(loaded_verts, loaded_elements);
    break;
  }
  return MeshDrawInfo(vao, (u32)loaded_elements.size(), all_loaded_textures, .1f, aiMat4_to_glmMat4(*trans));
}

inline void processAnimNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_MeshInfo, std::map<string, BoneInfo>& out_BoneInfoMap) {
  for (u32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    out_MeshInfo.push_back(processAnimMesh(mesh, scene, &node->mTransformation, out_BoneInfoMap));
  }

  for (u32 i = 0; i < node->mNumChildren; ++i) {
    processAnimNode(node->mChildren[i], scene, out_MeshInfo, out_BoneInfoMap);
  }
}

// returns inverse global transform;
inline void LoadAnimGameObjectFromFile(
  std::vector<MeshDrawInfo>& out_MeshInfo, std::map<string, BoneInfo>& out_BoneInfoMap, std::string path)   {
  m_BoneCounter = 0;  // init to 0 when starting a new model load

  Assimp::Importer importer;
  int post_processing_flags = 0;
  //post processing -> http://assimp.sourceforge.net/lib_html/postprocess_8h.html
  //post_processing_flags |= aiProcess_JoinIdenticalVertices | aiProcess_Triangulate;
  post_processing_flags |= aiProcess_Triangulate;

  const aiScene* scene = importer.ReadFile(path, post_processing_flags);
  errorCheck(scene);
  //if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
  //  throw("failed to load scene");
  //}

  std::size_t the_last_slash = path.find_last_of("/\\") + 1;
  std::size_t the_last_dot = path.find_last_of(".");

  mModelDir = path.substr(0, the_last_slash);  // path to filename's dir

  mModelFileExtension = path.substr(
    static_cast<std::basic_string<char,
    std::char_traits<char>,
    std::allocator<char>>::size_type>(the_last_dot) + 1);  // get the file extension (type of file)

  mModelFileName = path.substr(
    the_last_slash,
    static_cast<std::basic_string<char,
    std::char_traits<char>,
    std::allocator<char>>::size_type>(the_last_dot) - the_last_slash);  // get the name of the file

  processAnimNode(scene->mRootNode, scene, out_MeshInfo, out_BoneInfoMap);
  
}

}  // end namespace ModelLoader
}  // end namespace AA
