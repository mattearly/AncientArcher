/*
OGLGraphics
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "OGLGraphics.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <sstream>
#include <iostream>
#include <memory>
#include <string>
#include <cstddef>
#include "Conversions.h"
#include "GameObject.h"
#include "Vertex.h"

namespace AA
{

OGLGraphics* OGLGraphics::getInstance()
{
  static std::unique_ptr<OGLGraphics> graphics = std::make_unique<OGLGraphics>();
  return graphics.get();
}

int OGLGraphics::loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path, bool pp_triangulate)
{
  //Assimp::Importer importer;
  //int post_processsing_flags = 0;
  //post_processsing_flags |= aiProcess_JoinIdenticalVertices;
  //if (pp_triangulate)
  //{
  //  post_processsing_flags |= aiProcess_Triangulate;
  //}
  //const aiScene* scene = importer.ReadFile(path, post_processsing_flags);
  //if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  //{
  //  std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
  //  return -1;  // failed to load scene
  //}

  ////mLastDir = path.substr(0, path.find_last_of("/\\") + 1);  // get the beginning of the filename before the last / or \\

  //processNode(scene->mRootNode, scene, out_MeshInfo);

  return 0;
}

//void OGLGraphics::processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_MeshInfo)
//{
//  for (unsigned int i = 0; i < node->mNumMeshes; ++i)
//  {
//    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//    out_MeshInfo.push_back(processMesh(mesh, scene));
//  }
//
//  for (unsigned int i = 0; i < node->mNumChildren; ++i)
//  {
//    processNode(node->mChildren[i], scene, out_MeshInfo);
//  }
//}

//MeshDrawInfo OGLGraphics::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//  // get all vertex data for this mesh
//  std::vector<Vertex> loadedVerts;
//  for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
//  {
//    const glm::vec4 tmpPos = glm::vec4( AA::Conversions::aiVec3_to_glmVec3(mesh->mVertices[i]), 0.f);
//    const glm::vec4 tmpNorm = glm::vec4(AA::Conversions::aiVec3_to_glmVec3(mesh->mNormals[i]), 0.f);
//    glm::vec4 tmpColor(1, 0, 0, 1);
//    if (mesh->mColors[0])
//    {
//      tmpColor = AA::Conversions::aiColor4_to_glmVec4(mesh->mColors[0][i]);
//      std::cout << "tmp color loaded: " << tmpColor.r << " " << tmpColor.g << " " << tmpColor.b << " " << tmpColor.a << '\n';
//    }
//
//    glm::vec2 tmpTexCoords(0);
//    if (mesh->mTextureCoords[0] != nullptr)
//    {
//      tmpTexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
//    }
//    loadedVerts.emplace_back(Vertex(tmpPos, tmpNorm, tmpColor, tmpTexCoords));
//  }
//
//
//  // get the indices to draw triangle faces with
//  std::vector<unsigned int> loadedElements;
//  for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
//  {
//    aiFace face = mesh->mFaces[i];
//    for (unsigned int j = 0; j < face.mNumIndices; ++j)
//    {
//      loadedElements.push_back(face.mIndices[j]);
//    }
//  }
//
//
//  // get the materials
//  const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//  std::vector<TextureInfo> loadedTextures;
//
//  std::vector<TextureInfo> textureUnitMaps;
//
//  if (loadMaterialTextures(material, aiTextureType_DIFFUSE, "TextureUnit", textureUnitMaps) == 0) // if succeeds in loading texture add it to loaded texutres
//  {
//    loadedTextures.insert(loadedTextures.end(), textureUnitMaps.begin(), textureUnitMaps.end());
//  }
//  else
//  {
//    std::cout << "failed to load aiTextureType_DIFFUSE\n";
//  }
//
//  //std::vector<TextureInfo> specMaps;
//  //loadMaterialTextures(material, aiTextureType_SPECULAR, "specular", specMaps);
//  //loadedTextures.insert(loadedTextures.end(), specMaps.begin(), specMaps.end());
//
//  //std::vector<TextureInfo> normMaps;
//  //loadMaterialTextures(material, aiTextureType_HEIGHT, "normal", normMaps);
//  //loadedTextures.insert(loadedTextures.end(), normMaps.begin(), normMaps.end());
//
//  //std::vector<TextureInfo> heightMaps;
//  //loadMaterialTextures(material, aiTextureType_AMBIENT, "height", heightMaps);
//  //loadedTextures.insert(loadedTextures.end(), heightMaps.begin(), heightMaps.end());
//
//  //std::vector<TextureInfo> colorMaps;
//  //loadMaterialTextures(material, aiTextureType_BASE_COLOR, "base color", colorMaps);
//  //loadedTextures.insert(loadedTextures.end(), colorMaps.begin(), colorMaps.end());
//
//  float shine;
//  if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shine))
//  {
//    shine = .5f;
//    std::cout << "didn't get material shininess, set to .5f\n";
//  }
//  else  // success
//  {
//    std::cout << "material shininess found! set to: " << shine << '\n';
//  }
//
//  aiColor4D spec;
//  if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec))
//  {
//    spec = aiColor4D(1.f, 1.f, 1.f, 1.f);
//    std::cout << "didn't get material specular, set to 1,1,1,1\n";
//  }
//  else  // success
//  {
//    std::cout << "material specular found!\n";
//  }
//
//  glm::vec4 specular = AA::Conversions::aiColor4_to_glmVec4(spec);
//
//
//  unsigned int VAO, VBO, EBO;
//  glGenVertexArrays(1, &VAO);
//  glGenBuffers(1, &VBO);
//  glGenBuffers(1, &EBO);
//
//  glBindVertexArray(VAO);
//  glBindBuffer(GL_ARRAY_BUFFER, VBO);
//  glBufferData(GL_ARRAY_BUFFER, loadedVerts.size() * sizeof(Vertex), &loadedVerts[0], GL_STATIC_DRAW);
//
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedElements.size() * sizeof(unsigned int), &loadedElements[0], GL_STATIC_DRAW);
//
//  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, Position)));
//  glEnableVertexAttribArray(0);
//
//  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));
//  glEnableVertexAttribArray(1);
//
//  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
//  glEnableVertexAttribArray(2);
//
//  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
//  glEnableVertexAttribArray(3);
//
//  glBindVertexArray(0);
//
//  return MeshDrawInfo(VAO, loadedElements, loadedTextures, shine, specular);
//}
//
//int OGLGraphics::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureInfo>& out_texInfo)
//{
//  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
//  {
//    aiString tmpstr;
//    mat->GetTexture(type, i, &tmpstr);
//
//    // routine to see if we already have this texture loaded
//    bool alreadyLoaded = false;
//    for (unsigned int j = 0; j < mTexturesLoaded.size(); ++j)
//    {
//      for (const auto& p : mTexturesLoaded)
//      {
//        if (p.path.data() == tmpstr.C_Str())
//        {
//          TextureInfo tmptexinfo;
//          tmptexinfo.id = p.id;
//          tmptexinfo.type = p.type;
//          tmptexinfo.path = "";
//          out_texInfo.push_back(tmptexinfo);
//          alreadyLoaded = true;
//          std::cout << "Texture [" << p.path.data() << "] already loaded.\n";
//        }
//      }
//    }
//
//    // wasn't loaded, load it
//    if (!alreadyLoaded)
//    {
//      TextureInfo tmptex;
//      std::string tmpPath =/* mLastDir +*/ tmpstr.C_Str();
//      tmptex.id = TexLoader::getInstance()->textureFromFile(tmpPath.c_str());
//      if (tmptex.id != 0)
//      {
//        tmptex.path = tmpstr.C_Str();
//        tmptex.type = typeName;
//        out_texInfo.push_back(tmptex);
//        mTexturesLoaded.push_back(tmptex);
//      }
//      else
//      {
//        return -1; // failed to load new texture
//      }
//    }
//  }
//  return 0;
//}
//
///** Render the meshes with the shader. Assumes Camera View Matrix is already set.
// *  @param[in] meshes to draw.
// *  @param[in] details about instances of the mesh to also render.
// *  @param[in] shader to use for mesh rendering pipeline.
// */
//void OGLGraphics::Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, const OGLShader& modelShader)
//{
//  // turn on depth test in case something else turned it off
//  glEnable(GL_DEPTH_TEST);
//
//  // go through all meshes in the this
//  for (auto m : meshes)
//  {
//    // go through all textures in this mesh
//    for (unsigned int i = 0; i < m.textures.size(); ++i)
//    {
//      // activate each texture
//      glActiveTexture(GL_TEXTURE0 + i);
//      // get the texture type
//      const std::string texType = m.textures[i].type;
//
//      //might not need shader.use() here
//      //modelShader.use();
//
//      // tell opengl to bind the texture to a model shader uniform var
//      glUniform1i(glGetUniformLocation(modelShader.getID(), ("material." + texType).c_str()), i);
//      glBindTexture(GL_TEXTURE_2D, m.textures[i].id);
//    }
//
//    //modelShader.setFloat("material.Shininess", m.shininess);
//    //modelShader.setVec4("material.Specular", m.specular);
//
//    // bind verts
//    glBindVertexArray(m.vao);
//    const GLsizei count = (GLsizei)m.elements.size();
//
//    // draw all the instances with their differing model matrices
//    for (const auto& instance : details)
//    {
//      modelShader.setMat4("model", instance.ModelMatrix);
//      glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
//    }
//  }
//
//  // unbind vert array
//  glBindVertexArray(0);
//  // reset to first texture
//  glActiveTexture(GL_TEXTURE0);
//}

}  // end namespace AA