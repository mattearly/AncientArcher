#include "AAOGLGraphics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <string>
#include <cstddef>
#include <Conversions.h>
#include "AAGameObject.h"

using namespace mearly;

AAOGLGraphics* AAOGLGraphics::getInstance()
{
  static std::unique_ptr<AAOGLGraphics> graphics = std::make_unique<AAOGLGraphics>();
  return graphics.get();
}

std::vector<MeshDrawInfo> AAOGLGraphics::loadGameObjectWithAssimp(std::string path, bool pp_triangulate)
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
    return mMeshDrawInfo;
  }
  mLastDir = path.substr(0, path.find_last_of("/\\") + 1);
  processNode(scene->mRootNode, scene);
  return mMeshDrawInfo;
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
    glm::vec3 tmpPos = Conversions::aiVec3_to_glmVec3(mesh->mVertices[i]);
    glm::vec3 tmpNorm = Conversions::aiVec3_to_glmVec3(mesh->mNormals[i]);
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

  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  std::vector<TextureInfo> loadedTextures;
  //aiColor4D diffuse;
  //if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
  //{
  //  //unsigned int id = 0;
  //  //std::string type;
  //  //std::string path;
  //  //glm::vec3 color = glm::vec3(0);
  //  glm::vec3 color = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
  //  TextureInfo rawColorMap;
  //  rawColorMap.id = 0;
  //  rawColorMap.type = "color";
  //  rawColorMap.path = "";
  //  rawColorMap.color = color;
  //  loadedTextures.push_back(rawColorMap);
  //}

  std::vector<TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
  loadedTextures.insert(loadedTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
  std::vector<TextureInfo> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
  loadedTextures.insert(loadedTextures.end(), specMaps.begin(), specMaps.end());
  std::vector<TextureInfo> normMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
  loadedTextures.insert(loadedTextures.end(), normMaps.begin(), normMaps.end());
  std::vector<TextureInfo> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "height");
  loadedTextures.insert(loadedTextures.end(), heightMaps.begin(), heightMaps.end());
  std::vector<TextureInfo> colorMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "base color");
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

  return MeshDrawInfo(VAO, /*VBO, EBO,*/ loadedTextures, loadedElements);
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
      //tmptex.id = TexLoader::getInstance()->textureFromFile(tmpstr.C_Str(), mLastDir);
      std::string tmpPath = mLastDir + tmpstr.C_Str();
      tmptex.id = TexLoader::getInstance()->textureFromFile(tmpPath.c_str());
      tmptex.path = tmpstr.C_Str();
      tmptex.type = typeName;
      outTexInfo.push_back(tmptex);
      mTexturesLoaded.push_back(tmptex);
    }
  }
  return outTexInfo;
}

///////////////////////TEXLOADER//////////////////////

TexLoader* TexLoader::getInstance()
{
  //static TexLoader* staticTexLoader = new TexLoader();
  static std::unique_ptr<TexLoader> staticTexLoader = std::make_unique<TexLoader>();
  return staticTexLoader.get();
}

/**
 * This code loads in a cube map texture.
 * @param[in] files       to the textures
 * @return    textureID   id to reference the loaded texture
 */
unsigned int TexLoader::loadCubeTexture(const std::vector<std::string>& files)
{
  if (files.size() != 6)
  {
    throw std::runtime_error("Not enough files for cube map. A cube map needs to include 6 textures.");
  }
  unsigned int texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
  stbi_set_flip_vertically_on_load(false); // tell stb_image.h to not flip loaded texture's on the y-axis.
  int width, height, nrChannel;
  std::size_t size = files.size();
  for (auto i = 0; i < size; ++i)
  {
    unsigned char* data = stbi_load(files[i].c_str(), &width, &height, &nrChannel, 0);
    if (data)
    {
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
      );
      stbi_image_free(data);
    }
    else
    {
      stbi_image_free(data);
      //throw std::runtime_error("A cubemap texture was not able to be loaded.");
      return 0;
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return texID;
}

unsigned int TexLoader::textureFromFile(const char* filepath, bool gamma)
{
  stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filepath, &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format{};
    if (nrComponents == 1)
    {
      format = GL_RED;
    }
    else if (nrComponents == 3)
    {
      format = GL_RGB;
    }
    else if (nrComponents == 4)
    {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << filepath << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

///////////////////VERTEX CONSTRUCTOR///////////////////////////

Vertex::Vertex(glm::vec3 pos, glm::vec2 texcoords, glm::vec3 norms) noexcept
  : Position(pos), TexCoords(texcoords), Normal(norms) {}
