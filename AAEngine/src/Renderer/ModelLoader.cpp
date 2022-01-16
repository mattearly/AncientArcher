#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Vertex.h"
#include "../Utility/Conversions.h"
#include "OpenGL/OGLGraphics.h"

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

namespace AA {

/// <summary>
/// Holds info for reusing models that are already loaded.
/// </summary>
struct RefModelInfo {
  uint32_t vao = 0;
  uint32_t numElements = 0;
  std::string path = "";
  std::unordered_map<uint32_t, std::string> textureDrawIds{};   // id:type
  int ref_count = 1;
};

static std::forward_list<RefModelInfo> AllLoadedModels;

/// <summary>
/// Checks the AllLoadedModels reference list and reuses models that have already been loaded.
/// </summary>
/// <param name="out_model">the model to be populated if successful</param>
/// <param name="path">full original path</param>
/// <returns>true if out_model was populated, false if not</returns>
bool ModelLoader::CheckIfModelIsAlreadyLoaded(Prop& out_model, const std::string& path) {
  for (auto& model : AllLoadedModels) {
    if (model.path == "")
      continue;
    if (path == model.path.data()) {
#ifdef _DEBUG
      std::cout << "-> REUSE... PROP: " << path << std::endl;
#endif
      model.ref_count++;
      TextureLoader::increment_given_texture_ids(model.textureDrawIds);
      out_model.mMeshes.emplace_back(MeshInfo(model.vao, model.numElements, model.textureDrawIds, glm::mat4(1)));
      return true;  // already loaded
    }
  }
  return false; // not already loaded
}

static std::string LastLoadedPath;

// Loads the data from the model at path, and saves it to out_model
// returns true on fail to open:
//   error codes: 
//    -1 = scene was null after attempt to load
//    -2 = scene has incomplete flag from assimp after attempted to load
//    -3 = there is no root node on the model
// otherwise returns 0 if the import is successful
int ModelLoader::LoadGameObjectFromFile(Prop& out_model, const std::string& path) {

  bool already_loaded = CheckIfModelIsAlreadyLoaded(out_model, path);
  if (already_loaded) return 0;


#ifdef _DEBUG
  std::cout << "-> LOAD... PROP: " << path << std::endl;
#endif

  Assimp::Importer importer;
  int post_processing_flags = 0;

  //post processing -> http://assimp.sourceforge.net/lib_html/postprocess_8h.html
  post_processing_flags |= aiProcess_JoinIdenticalVertices |
    aiProcess_Triangulate |
    aiProcess_FlipUVs |
    //#ifdef D3D
    //	aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | 
    //#endif
    //aiProcess_PreTransformVertices |
    //aiProcess_CalcTangentSpace |
    //aiProcess_GenSmoothNormals |
    //aiProcess_FixInfacingNormals |
    //aiProcess_FindInvalidData |
    aiProcess_ValidateDataStructure
    ;

  const aiScene* scene = importer.ReadFile(path, post_processing_flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    if (!scene)
#ifdef _DEBUG
      std::cout << "scene loading error: " << importer.GetErrorString() << std::endl;
#endif
    return -1;
    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
      return -2;
    if (!scene->mRootNode)
      return -3;
  }

  LastLoadedPath = path;

  recursive_processNode(scene->mRootNode, scene, out_model);

  // if it loaded correctly, save a ref to this loaded up model (certainly a new entry at this point because we checked for copy at the begginning of this function LoadGameObjectFromFile)
  RefModelInfo temp_mesh_info;
  temp_mesh_info.vao = out_model.mMeshes.front().vao;
  temp_mesh_info.numElements = out_model.mMeshes.front().numElements;
  temp_mesh_info.path = path;
  temp_mesh_info.textureDrawIds = out_model.mMeshes.front().textureDrawIds;   // id:type
  AllLoadedModels.push_front(temp_mesh_info);

  return 0;
}


// unloads all textures and vao's from a list of nodeinfos
void ModelLoader::UnloadGameObject(const std::vector<MeshInfo>& toUnload) {
  for (const auto& a_mesh : toUnload) {
    OGLGraphics::DeleteMesh(a_mesh.vao);
    TextureLoader::UnloadTexture(a_mesh.textureDrawIds);
  }
}

/// <summary>
/// 4 step helper function for LoadGameObjectFromFile & recursive_processNode
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
/// <param name="trans"></param>
/// <returns>MeshInfo: data for drawing a Mesh</returns>
MeshInfo local_helper_processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans) {
  // Sanity Check
  unsigned int num_of_vertices_on_mesh = mesh->mNumVertices;
  if (num_of_vertices_on_mesh == 0)
    throw("mesh has no vertices");

  // Step1: Get the Vertices
  std::vector<LitVertex> loaded_vertices;
  if (mesh->mTextureCoords[0]) { // case for when texture coordinantes exist
    for (unsigned int i = 0; i < num_of_vertices_on_mesh; ++i) {
      loaded_vertices.emplace_back(LitVertex(aiVec3_to_glmVec3(mesh->mVertices[i]), glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y), aiVec3_to_glmVec3(mesh->mNormals[i])));
    }
  } else {  // case for when texture coordinantes do not exist
    for (unsigned int i = 0; i < num_of_vertices_on_mesh; ++i) {
      loaded_vertices.emplace_back(LitVertex(aiVec3_to_glmVec3(mesh->mVertices[i]), glm::vec2(0), aiVec3_to_glmVec3(mesh->mNormals[i])));
    }
  }

  // Step2: Get the Indices to draw triangle faces with
  std::vector<unsigned int> loaded_elements;
  unsigned int num_faces = mesh->mNumFaces;
  for (unsigned int i = 0; i < num_faces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      loaded_elements.push_back(face.mIndices[j]);
    }
  }

  // Step3: Send the vertex data to the graphic memory
  unsigned int vao = 0;
  vao = OGLGraphics::Upload3DMesh(loaded_vertices, loaded_elements);

  // Step4: Return this draw data to the user
  return MeshInfo(
    vao,
    (unsigned int)loaded_elements.size(),
    TextureLoader::LoadAllTextures(scene, scene->mMaterials[mesh->mMaterialIndex], LastLoadedPath),  // get all the textures that belong with this mesh
    aiMat4_to_glmMat4(*trans)
  );
}


// recursive helper function for LoadGameObjectFromFile
void ModelLoader::recursive_processNode(aiNode* node, const aiScene* scene, Prop& out_model) {
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    out_model.mMeshes.push_back(local_helper_processMesh(mesh, scene, &node->mTransformation));
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    recursive_processNode(node->mChildren[i], scene, out_model);
  }
}

} // end namespace AA
