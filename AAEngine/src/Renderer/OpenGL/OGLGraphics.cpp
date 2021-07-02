#include "OGLGraphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image.h>
#include <glad/glad.h>
#include <memory>
#include <string>
#include <cstddef>

namespace AA {
namespace OGLGraphics {

void SetSamplerCube(int which, const int& cubetexID) {
  glActiveTexture(GL_TEXTURE0 + which);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubetexID);
}

void SetTexture(int which, const int& textureID) {
  glActiveTexture(GL_TEXTURE0 + which);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void RenderSkybox(const int& vao, const int& count) {
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, count);

  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

void Render(const std::vector<MeshDrawInfo>& meshes, const glm::mat4& translationMatrix, SHADERTYPE shadertype) {
  glEnable(GL_DEPTH_TEST);

  switch (shadertype) {
  case SHADERTYPE::DIFF:
    mDiffShader->Use();
    break;
  case SHADERTYPE::LIT:
    mLitShader->Use();
    break;
  }

  // go through all meshes in the this
  for (const auto& m : meshes) {
    // update translationMatrix
    switch (shadertype) {
    case SHADERTYPE::DIFF:
      mDiffShader->SetMat4("u_model_matrix", /*m.transformation * */translationMatrix);
      break;
    case SHADERTYPE::LIT:
      mLitShader->SetFloat("material.Shininess", m.shininess);
      mLitShader->SetMat4("u_model_matrix", /*m.transformation * */translationMatrix);
      break;
    }

    // go through all texture in this mesh
    u32 i = 0;
    for (const auto& texture : m.textureDrawIds) {
      const std::string texType = texture.second;  // get the texture type
      switch (shadertype) {
      case SHADERTYPE::DIFF:
        if (texType == "Albedo") {
          OGLGraphics::SetTexture(i, texture.first);  // activate texture & bind it
          mDiffShader->SetInt(("material." + texType).c_str(), i);
        }
        break;
      case SHADERTYPE::LIT:
        if (texType == "Albedo" || texType == "Specular") {
          OGLGraphics::SetTexture(i, texture.first);  // activate texture & bind it
          mLitShader->SetInt(("material." + texType).c_str(), i);
        }
        break;
      }
      i++;
    }
    glBindVertexArray(m.vao);
    glDrawElements(GL_TRIANGLES, m.numElements, GL_UNSIGNED_INT, nullptr);
  }
  // unbind vert array
  glBindVertexArray(0);
  // reset to first texture
  glActiveTexture(GL_TEXTURE0);
}

void Render(const MeshDrawInfo& mesh, SHADERTYPE shadertype) {
  glEnable(GL_DEPTH_TEST);

  switch (shadertype) {
  case SHADERTYPE::DIFF:
    mDiffShader->Use();
    mDiffShader->SetMat4("u_model_matrix", mesh.transformation);
    break;
  case SHADERTYPE::LIT:
    mLitShader->Use();
    mLitShader->SetFloat("material.Shininess", mesh.shininess);
    mLitShader->SetMat4("u_model_matrix", mesh.transformation);
    break;
  }

  // go through all texture in this mesh
  u32 i = 0;
  for (const auto& texture : mesh.textureDrawIds) {
    const std::string texType = texture.second;  // get the texture type
    switch (shadertype) {
    case SHADERTYPE::DIFF:
      if (texType == "Albedo") {
        OGLGraphics::SetTexture(i, texture.first);  // activate texture & bind it
        mDiffShader->SetInt(("material." + texType).c_str(), i);
      }
      break;
    case SHADERTYPE::LIT:
      if (texType == "Albedo" || texType == "Specular") {
        OGLGraphics::SetTexture(i, texture.first);  // activate texture & bind it
        mLitShader->SetInt(("material." + texType).c_str(), i);
      }
      break;
    }
    i++;
  }
  glBindVertexArray(mesh.vao);
  glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, nullptr);
  // unbind vert array
  glBindVertexArray(0);
  // reset to first texture
  glActiveTexture(GL_TEXTURE0);
}

void RenderStrip(const int& vao, const int& count) {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
  glBindVertexArray(0);
}

void SetViewportSize(int x, int y, int w, int h) {
  glViewport(x, y, w, h);
}

/// <summary>
/// sets the color of the opengl viewport if nothing is drawn
/// </summary>
/// <param name="vec3">rgb floats between 0 and 1</param>
/// <returns></returns>
void SetViewportClearColor(glm::vec3 color) noexcept {
  glClearColor(color.x, color.y, color.z, 0.0f);
}

/// <summary>
/// pre-render function
/// </summary>
void ClearScreen()  noexcept {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// <summary>
/// sends the mesh data to the graphics card
/// </summary>
/// <param name="points">array of points to upload</param>
/// <param name="num_points">number in the array</param>
/// <returns>the VAO</returns>
u32 UploadMesh(const float* points, const int num_points) {
  u32 VAO, VBO;

  //todo: use element buffers
  glGenVertexArrays(1, &VAO);

  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(*points) * num_points, points, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  return VAO;
}

/// <summary>
/// sends the mesh data to the graphics card
/// </summary>
/// <param name="verts">vertices to upload</param>
/// <param name="elems">relevant indicies</param>
/// <returns>the VAO</returns>
u32 UploadMesh(const std::vector<Vertex>& verts, const std::vector<u32>& elems) {
  u32 VAO, VBO, EBO;
  glGenBuffers(1, &VBO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Tangent));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, BiTangent));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elems.size() * sizeof(u32), &elems[0], GL_STATIC_DRAW);

  glBindVertexArray(0);

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  return VAO;
}

u32 UploadMesh(const float* points, const int num_points, const u32* indices, const int ind_count) {
  u32 VAO, VBO, EBO;
  glGenBuffers(1, &VBO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,num_points * sizeof(float), points, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

  glEnableVertexAttribArray(0);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind_count * sizeof(u32), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  return VAO;
}


u32 Upload2DVerts(const std::vector<vec2>& points) {
  u32 VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  std::vector<float> data;
  data.reserve(points.size() * 2);
  for (const auto& p : points) {
    data.push_back(p.x);
    data.push_back(p.y);
  }
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  return VAO;
}

u32 Upload2DVerts(const std::vector<float>& points) {
  u32 VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  return VAO;
}

/// <summary>
/// removes the mesh from our GPU memory
/// </summary>
/// <param name="VAO">vao to delete</param>
void DeleteMesh(const u32& VAO) {
  glDeleteBuffers(1, &VAO);
}

u32 Upload2DTex(const unsigned char* tex_data, int width, int height, bool hasAlpha) {
  unsigned int out_texID = 0;
  glGenTextures(1, &out_texID);
  glBindTexture(GL_TEXTURE_2D, out_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //try: https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);

  if (hasAlpha) {
    //https://youtu.be/n4k7ANAFsIQ?t=910
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, /*border*/0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, /*border*/0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
  }

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return out_texID;
}

u32 UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height, bool hasAlpha) {
  unsigned int out_texID;
  glGenTextures(1, &out_texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, out_texID);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


  if (hasAlpha) {
    for (auto i = 0; i < 6; ++i) {
      if (tex_data[i]) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data[i]);
      }
    }
  } else {
    for (auto i = 0; i < 6; ++i) {
      if (tex_data[i]) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data[i]);
      }
    }
  }

  return out_texID;
}

void DeleteTex(const u32& id) {
  glDeleteTextures(1, &id);
}

void SetMSAA(const bool enabled) {
  glEnable(GL_MULTISAMPLE);
}

void SetBlend(const bool enabled) {
  if (enabled) {
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }
}

void SetCullFace(const bool enabled) {
  if (enabled)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
}

}  // end namespace OGLGraphics
}  // end namespace AA