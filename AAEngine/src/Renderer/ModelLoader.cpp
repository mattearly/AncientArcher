#include "ModelLoader.h"

#include "../../include/AncientArcher/Utility/SceneCheck.h"
#include "../Scene/Vertex.h"
#include "../Settings/Settings.h"
#include "../Utility/Conversions.h"
#include "OpenGL/OGLGraphics.h"
#include "TextureLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image.h>

#include <sstream>
#include <utility>
#include <forward_list>
#include <unordered_map>
#include <map>
#ifdef _DEBUG
#include <iostream>
#endif

namespace AA{
namespace ModelLoader{

string mModelDir;
string mModelFileName;
string mModelFileExtension;

void UnloadGameObject(const std::vector<MeshDrawInfo>& toUnload) {
  for (const auto& meshIt : toUnload) {
    switch (Settings::Get()->GetOptions().renderer) {
    case RenderingFramework::OPENGL:
      OGLGraphics::DeleteMesh(meshIt.vao);
      for (const auto& texIt : meshIt.textureDrawIds) {
        for (auto loaded_tex = TextureLoader::mLoadedTextures.begin(); loaded_tex != TextureLoader::mLoadedTextures.end(); loaded_tex++) {
          if (texIt.first == loaded_tex->accessId) {
            loaded_tex->ref_count--;
            if (loaded_tex->ref_count == 0) {
              OGLGraphics::DeleteTex(loaded_tex->accessId);
            }
          }
        }

        // sync local cube_loaded textures list
        TextureLoader::mLoadedTextures.remove_if([](const TextureInfo& ti) -> bool { return (ti.ref_count == 0) ? true : false; });
      }
      break;
    }
  }
}

void processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans, Prop& out_model) {
  u32 num_of_vertices_on_mesh = mesh->mNumVertices;
  if (num_of_vertices_on_mesh == 0)
    throw("mesh has no vertices");

  std::vector<Vertex> loaded_vertices;
  VertexBoneData bone_data{};

  // Get the vertices
  if (mesh->mTextureCoords[0]) {
    for (u32 i = 0; i < num_of_vertices_on_mesh; ++i) {
      loaded_vertices.emplace_back(
        Vertex(
          aiVec3_to_glmVec3(mesh->mVertices[i]), 
          aiVec3_to_glmVec3(mesh->mNormals[i]),
          vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)));
    }
  } else {
    for (u32 i = 0; i < num_of_vertices_on_mesh; ++i) {
      loaded_vertices.emplace_back(
        Vertex(
          aiVec3_to_glmVec3(mesh->mVertices[i]), 
          aiVec3_to_glmVec3(mesh->mNormals[i])));
    }
  }

  // Get the Indices to draw triangle faces with
  std::vector<u32> loaded_elements;
  u32 num_faces = mesh->mNumFaces;
  for (u32 i = 0; i < num_faces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (u32 j = 0; j < face.mNumIndices; ++j) {
      loaded_elements.push_back(face.mIndices[j]);
    }
  }

  // load bones
  if (out_model.isAnimated) {
    for (u32 i = 0; i < mesh->mNumBones; i++) {
      u32 BoneIndex = 0;
      string BoneName(mesh->mBones[i]->mName.data);

      if (out_model.mBoneMapping.find(BoneName) == out_model.mBoneMapping.end()) {
        // Allocate an index for a new bone
        BoneIndex = out_model.mNumBones;
        out_model.mNumBones++;
        BoneInfo bi;
        out_model.mBoneInfo.push_back(bi);
        out_model.mBoneInfo[BoneIndex].BoneOffset = aiMat4_to_glmMat4(mesh->mBones[i]->mOffsetMatrix);
        out_model.mBoneMapping[BoneName] = BoneIndex;
      } else {
        BoneIndex = out_model.mBoneMapping[BoneName];
      }

      for (u32 j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) {
        u32 VertexID = /*out_model.mMeshes[meshIndex].BaseVertex + */mesh->mBones[i]->mWeights[j].mVertexId;
        float Weight  = mesh->mBones[i]->mWeights[j].mWeight;
        for (u32 k = 0 ; k < 4 ; k++) {
          if (out_model.mMeshes[VertexID].boneData.Weights[k] == 0.0f) {
            out_model.mMeshes[VertexID].boneData.IDs[k] = BoneIndex;
            out_model.mMeshes[VertexID].boneData.Weights[k] = Weight;
            break; // ???????????????????????????????????????????????
          }
        }
      }
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
#if _DEBUG
      std::cout << "found&loaded Albedo texture\n";
#endif
    }
  }

  //ai_real shininess = .1f;
  //int shading_model;
  //ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_model);
//  if (shading_model == aiShadingMode_Phong) {
//#if _DEBUG
//    std::cout << mModelFileName << ": ";
//    std::cout << "Shading model is phong\n";
//#endif
    std::unordered_map<u32, std::string> specular_textures;
    if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_SPECULAR, "Specular", specular_textures) == 0) {
      for (auto& s_tex : specular_textures) {
        all_loaded_textures.insert(all_loaded_textures.end(), s_tex);
#if _DEBUG
        std::cout << "found&loaded Specular texture\n";
#endif
      }
    }

    std::unordered_map<u32, std::string> normal_textures;
    if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_NORMALS, "Normal", normal_textures) == 0) {
      for (auto& s_tex : normal_textures) {
        all_loaded_textures.insert(all_loaded_textures.end(), s_tex);
#if _DEBUG
        std::cout << "found&loaded Normal texture\n";
#endif
      }
    //}
//    if (!ai_material->Get(AI_MATKEY_SHININESS, shininess)) {
//      // set shininess to a default if it failed
//#if _DEBUG
//      std::cout << "shininess not found, shininess defaulted to 1.f\n";
//#endif
//    }
  }

  u32 vao = 0;
  switch (Settings::Get()->GetOptions().renderer) {
  case RenderingFramework::OPENGL:
    vao = OGLGraphics::UploadMesh(loaded_vertices, loaded_elements);
    break;
  }

  if (out_model.isAnimated)
    out_model.mMeshes.push_back(MeshDrawInfo(vao, (u32)loaded_elements.size(), all_loaded_textures, /*shininess*/ 1, aiMat4_to_glmMat4(*trans), bone_data));
  else 
    out_model.mMeshes.push_back(MeshDrawInfo(vao, (u32)loaded_elements.size(), all_loaded_textures, /*shininess*/ 1, aiMat4_to_glmMat4(*trans)));

}

void processNode(aiNode* node, const aiScene* scene, Prop& out_model) {
  for (u32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    //out_model.mMeshes.push_back(processMesh(mesh, scene, &node->mTransformation, out_model.isAnimated));

    processMesh(mesh, scene, &node->mTransformation, out_model);
  }

  for (u32 i = 0; i < node->mNumChildren; ++i) {
    processNode(node->mChildren[i], scene, out_model);
  }
}

int LoadGameObjectFromFile(Prop& out_model, string path, bool animate) {
  Assimp::Importer importer;
  int post_processing_flags = 0;
  //post processing -> http://assimp.sourceforge.net/lib_html/postprocess_8h.html
  post_processing_flags |= /*aiProcess_JoinIdenticalVertices |*/ aiProcess_Triangulate/*;*/
   |
  #ifdef D3D
  	aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs |
  #endif
  //aiProcess_PreTransformVertices |
  //aiProcess_CalcTangentSpace |
  aiProcess_GenSmoothNormals /*|*/;
  //aiProcess_Triangulate |
  //aiProcess_FixInfacingNormals |
  //aiProcess_FindInvalidData |
  //aiProcess_ValidateDataStructure;

  const aiScene* scene = importer.ReadFile(path, post_processing_flags);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    //std::cout << "failed to load scene @ " << path << '\n';
    return -1;  // failed to load scene
  }

  if (scene && animate) {
    out_model.isAnimated = true;
    out_model.mGlobalInverseTransform = glm::inverse(aiMat4_to_glmMat4(scene->mRootNode->mTransformation));
  } else {
    throw("model didn't load");
  }

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

  processNode(scene->mRootNode, scene, out_model);

  return 0;
}

}
}
