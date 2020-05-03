#include "SceneLoader.h"
#include <iostream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Vertex.h"
#include "../Conversions.h"
#include "../TexLoader.h"
#include <glad/glad.h>
namespace AA
{

int SceneLoader::loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path, bool pp_triangulate)
{
  Assimp::Importer importer;
  int post_processsing_flags = 0;
  post_processsing_flags |= aiProcess_JoinIdenticalVertices;
  if (pp_triangulate)
  {
    post_processsing_flags |= aiProcess_Triangulate;
  }
  const aiScene* scene = importer.ReadFile(path, post_processsing_flags);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
    return -1;  // failed to load scene
  }

  //mLastDir = path.substr(0, path.find_last_of("/\\") + 1);  // get the beginning of the filename before the last / or \\

  //std::string getBasePath(const std::string & path)
  //{
  //  size_t pos = path.find_last_of("\\/");
  //  return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
  //}

  processNode(scene->mRootNode, scene, out_MeshInfo);

  return 0;
}

void SceneLoader::processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_MeshInfo)
{

  for (unsigned int i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    out_MeshInfo.push_back(processMesh(mesh, scene, &node->mTransformation));
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i)
  {
    processNode(node->mChildren[i], scene, out_MeshInfo);
  }
}

MeshDrawInfo SceneLoader::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans)
{

  // get all vertex data for this mesh
  std::vector<Vertex> loadedVerts;
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
  {
    const glm::vec4 tmpPos = glm::vec4(Conversions::aiVec3_to_glmVec3(mesh->mVertices[i]), 0.f);
    const glm::vec4 tmpNorm = glm::vec4(Conversions::aiVec3_to_glmVec3(mesh->mNormals[i]), 0.f);
    glm::vec4 tmpColor(1, 0, 0, 1);
    if (mesh->mColors[0])
    {
      tmpColor = Conversions::aiColor4_to_glmVec4(mesh->mColors[0][i]);
      std::cout << "tmp color loaded: " << tmpColor.r << " " << tmpColor.g << " " << tmpColor.b << " " << tmpColor.a << '\n';
    }

    glm::vec2 tmpTexCoords(0);
    if (mesh->mTextureCoords[0] != nullptr)
    {
      tmpTexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    loadedVerts.emplace_back(Vertex(tmpPos, tmpNorm, tmpColor, tmpTexCoords));
  }


  // get the indices to draw triangle faces with
  std::vector<unsigned int> loadedElements;
  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j)
    {
      loadedElements.push_back(face.mIndices[j]);
    }
  }


  // get the materials
  const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<TextureInfo> loadedTextures;

  std::vector<TextureInfo> textureUnitMaps;

  if (loadMaterialTextures(material, aiTextureType_DIFFUSE, "TextureUnit", textureUnitMaps) == 0) // if succeeds in loading texture add it to loaded texutres
  {
    loadedTextures.insert(loadedTextures.end(), textureUnitMaps.begin(), textureUnitMaps.end());
  }
  else
  {
    std::cout << "failed to load aiTextureType_DIFFUSE\n";
  }

  //std::vector<TextureInfo> specMaps;
  //loadMaterialTextures(material, aiTextureType_SPECULAR, "specular", specMaps);
  //loadedTextures.insert(loadedTextures.end(), specMaps.begin(), specMaps.end());

  //std::vector<TextureInfo> normMaps;
  //loadMaterialTextures(material, aiTextureType_HEIGHT, "normal", normMaps);
  //loadedTextures.insert(loadedTextures.end(), normMaps.begin(), normMaps.end());

  //std::vector<TextureInfo> heightMaps;
  //loadMaterialTextures(material, aiTextureType_AMBIENT, "height", heightMaps);
  //loadedTextures.insert(loadedTextures.end(), heightMaps.begin(), heightMaps.end());

  //std::vector<TextureInfo> colorMaps;
  //loadMaterialTextures(material, aiTextureType_BASE_COLOR, "base color", colorMaps);
  //loadedTextures.insert(loadedTextures.end(), colorMaps.begin(), colorMaps.end());

  float shine;
  if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shine))
  {
    shine = .5f;
    std::cout << "didn't get material shininess, set to .5f\n";
  }
  else  // success
  {
    std::cout << "material shininess found! set to: " << shine << '\n';
  }

  aiColor4D spec;
  glm::vec4 specular;
  if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec))
  {
    specular = glm::vec4(1.f);
    std::cout << "didn't get material specular, set to 1,1,1,1\n";  //debug
  }
  else  // success
  {
    specular = Conversions::aiColor4_to_glmVec4(spec);
    std::cout << "material specular found! set to " 
      << specular.r << ','
      << specular.g << ','
      << specular.b << ','
      << specular.a << '\n';  // debug
  }

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, loadedVerts.size() * sizeof(Vertex), &loadedVerts[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedElements.size() * sizeof(unsigned int), &loadedElements[0], GL_STATIC_DRAW);

  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, Position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  return MeshDrawInfo(VAO, loadedElements, loadedTextures, shine, specular, Conversions::aiMat4_to_glmMat4(*trans));
}

int SceneLoader::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureInfo>& out_texInfo)
{
  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
  {
    aiString tmpstr;
    mat->GetTexture(type, i, &tmpstr);

    // routine to see if we already have this texture loaded
    bool alreadyLoaded = false;
    for (unsigned int j = 0; j < mTexturesLoaded.size(); ++j)
    {
      for (const auto& p : mTexturesLoaded)
      {
        if (p.path.data() == tmpstr.C_Str())
        {
          TextureInfo tmptexinfo;
          tmptexinfo.id = p.id;
          tmptexinfo.type = p.type;
          tmptexinfo.path = "";
          out_texInfo.push_back(tmptexinfo);
          alreadyLoaded = true;
          std::cout << "Texture [" << p.path.data() << "] already loaded.\n";
        }
      }
    }

    // wasn't loaded, load it
    if (!alreadyLoaded)
    {
      TextureInfo tmptex;
      std::string tmpPath =/* mLastDir +*/ tmpstr.C_Str();
      tmptex.id = TexLoader::getInstance()->textureFromFile(tmpPath.c_str());
      if (tmptex.id != 0)
      {
        tmptex.path = tmpstr.C_Str();
        tmptex.type = typeName;
        out_texInfo.push_back(tmptex);
        mTexturesLoaded.push_back(tmptex);
      }
      else
      {
        return -1; // failed to load new texture
      }
    }
  }
  return 0;
}
} // end namespace AA