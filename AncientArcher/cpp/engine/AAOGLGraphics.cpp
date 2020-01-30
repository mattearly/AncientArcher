#include "AAOGLGraphics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <mearly\TexLoader.h>
#include <glad\glad.h>
#include <cstddef>
#include "AAViewport.h"

Vertex::Vertex(glm::vec3 pos, glm::vec2 texcoords, glm::vec3 norms) noexcept
  : Position(pos), TexCoords(texcoords), Normal(norms) {}

glm::mat4 aiMat4_to_glmMat4(const aiMatrix4x4& inMat)
{
  glm::mat4 outMat;
  outMat[0][0] = inMat.a1;
  outMat[1][0] = inMat.b1;
  outMat[2][0] = inMat.c1;
  outMat[3][0] = inMat.d1;
  outMat[0][1] = inMat.a2;
  outMat[1][1] = inMat.b2;
  outMat[2][1] = inMat.c2;
  outMat[3][1] = inMat.d2;
  outMat[0][2] = inMat.a3;
  outMat[1][2] = inMat.b3;
  outMat[2][2] = inMat.c3;
  outMat[3][2] = inMat.d3;
  outMat[0][3] = inMat.a4;
  outMat[1][3] = inMat.b4;
  outMat[2][3] = inMat.c4;
  outMat[3][3] = inMat.d4;
  return outMat;
}

glm::vec3 aiVec3_to_glmVec3(const aiVector3D& inVec) noexcept
{
  glm::vec3 outVec;
  outVec.x = inVec.x;
  outVec.y = inVec.y;
  outVec.z = inVec.z;
  return outVec;
}

glm::quat aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept
{
  glm::quat outQuat;
  outQuat.w = inQuat.w;
  outQuat.x = inQuat.x;
  outQuat.y = inQuat.y;
  outQuat.z = inQuat.z;
  return outQuat;
}

AAOGLGraphics* AAOGLGraphics::getInstance()
{
  static AAOGLGraphics* graphics = new AAOGLGraphics();
  return graphics;
}

AAGameObject AAOGLGraphics::loadGameObjectWithAssimp(std::string path, bool pp_triangulate)
{
  Assimp::Importer importer;
  int post_processsing_flags = 0;
  post_processsing_flags |= aiProcess_JoinIdenticalVertices;
  if (pp_triangulate)
  {
    post_processsing_flags |= aiProcess_Triangulate;
  }
  const aiScene* scene = importer.ReadFile(path, post_processsing_flags);
  mMeshDrawInfo.clear();
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
    return AAGameObject(mMeshDrawInfo);
  }

  mLastDir = path.substr(0, path.find_last_of("/\\"));  //http://www.cplusplus.com/reference/string/string/find_last_of/     
  processNode(scene->mRootNode, scene);

  return AAGameObject(mMeshDrawInfo);
}

void AAOGLGraphics::processNode(aiNode* node, const aiScene* scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    mMeshDrawInfo.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i)
  {
    processNode(node->mChildren[i], scene);
  }
}

MeshDrawInfo AAOGLGraphics::processMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> loadedVerts;
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
  {
    glm::vec3 tmpPos = aiVec3_to_glmVec3(mesh->mVertices[i]);
    glm::vec3 tmpNorm = aiVec3_to_glmVec3(mesh->mNormals[i]);
    glm::vec2 tmpTexCoords(0);
    if (mesh->mTextureCoords[0] != nullptr)
    {
      tmpTexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    loadedVerts.emplace_back(Vertex(tmpPos, tmpTexCoords, tmpNorm));
  }

  std::vector<unsigned int> loadedElements;
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j)
    {
      loadedElements.push_back(face.mIndices[j]);
    }
  }

  std::vector<TextureInfo> loadedTextures;
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  std::vector<TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
  loadedTextures.insert(loadedTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
  std::vector<TextureInfo> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
  loadedTextures.insert(loadedTextures.end(), specMaps.begin(), specMaps.end());
  std::vector<TextureInfo> normMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
  loadedTextures.insert(loadedTextures.end(), normMaps.begin(), normMaps.end());
  std::vector<TextureInfo> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "height");
  loadedTextures.insert(loadedTextures.end(), heightMaps.begin(), heightMaps.end());
  std::vector<TextureInfo> colorMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "color");
  loadedTextures.insert(loadedTextures.end(), colorMaps.begin(), colorMaps.end());

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, loadedVerts.size() * sizeof(Vertex), &loadedVerts[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedElements.size() * sizeof(unsigned int), &loadedElements[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

  glBindVertexArray(0);

  return MeshDrawInfo(VAO, VBO, EBO, loadedTextures, loadedElements);
}

std::vector<TextureInfo> AAOGLGraphics::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
  std::vector<TextureInfo> outTexInfo;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
  {
    aiString tmpstr;
    mat->GetTexture(type, i, &tmpstr);
    bool alreadyLoaded = false;

    for (unsigned int j = 0; j < mTexturesLoaded.size(); ++j)
    {
      for (auto p : mTexturesLoaded)
      {
        if (p.path.data() == tmpstr.C_Str())
        {
          TextureInfo tmptexinfo;
          tmptexinfo.id = p.id;
          tmptexinfo.type = p.type;
          tmptexinfo.path = "";
          outTexInfo.push_back(tmptexinfo);
          alreadyLoaded = true;
        }
      }
    }

    if (!alreadyLoaded)
    {
      TextureInfo tmptex;
      tmptex.id = TexLoader::getInstance()->textureFromFile(tmpstr.C_Str(), mLastDir);
      tmptex.path = tmpstr.C_Str();
      tmptex.type = typeName;
      outTexInfo.push_back(tmptex);
      mTexturesLoaded.push_back(tmptex);
    }
  }

  return outTexInfo;
}
