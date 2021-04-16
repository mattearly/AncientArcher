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
void Render(const std::vector<MeshDrawInfo>& meshes, const glm::mat4& translationMatrix, SHADERTYPE shadertype) {
  glEnable(GL_DEPTH_TEST);

  switch (shadertype) {
  case SHADERTYPE::DIFF:
    mDiffShader->use();
    break;
  case SHADERTYPE::LIT:
    mLitShader->use();
    break;
  }

  // go through all meshes in the this
  for (const auto& m : meshes) {
    // go through all texture in this mesh
    uint32_t i = 0;
    switch (shadertype) {
    case SHADERTYPE::DIFF:
      mDiffShader->setMat4("model", m.transformation * translationMatrix);
      break;
    case SHADERTYPE::LIT:
      mLitShader->setFloat("material.Shininess", m.shininess);
      mLitShader->setMat4("model", m.transformation * translationMatrix);
      break;
    }

    for (const auto& texture : m.textureDrawIds) {
      // activate each texture
      glActiveTexture(GL_TEXTURE0 + i);
      // get the texture type
      const std::string texType = texture.second;
      switch (shadertype) {
      case SHADERTYPE::DIFF:
        if (texType == "Albedo") {
          mDiffShader->setInt(("material." + texType).c_str(), i);
        }        break;
      case SHADERTYPE::LIT:
        mLitShader->setInt(("material." + texType).c_str(), i);
        break;
      }
      glBindTexture(GL_TEXTURE_2D, texture.first);
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

void RenderStrip(const int& vao, const int& count) {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
  glBindVertexArray(0);
}

/// <summary>
/// Sets the size of the OpenGL viewport
/// </summary>
/// <param name="x">starting loc</param>
/// <param name="y">starting loc</param>
/// <param name="w">width</param>
/// <param name="h">height</param>
void OGLGraphics::SetViewportSize(int x, int y, int w, int h) {
  glViewport(x, y, w, h);
}

/// <summary>
/// sets the color of the opengl viewport if nothing is drawn
/// </summary>
/// <param name="vec3">rgb floats between 0 and 1</param>
/// <returns></returns>
void OGLGraphics::SetViewportClearColor(glm::vec3 color) noexcept {
  glClearColor(color.x, color.y, color.z, 0.0f);
}

/// <summary>
/// pre-render function
/// </summary>
void OGLGraphics::ClearScreen()  noexcept {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// <summary>
/// sends the mesh data to the graphics card
/// </summary>
/// <param name="verts">vertices to upload</param>
/// <param name="elems">relevant indicies</param>
/// <returns>the VAO</returns>
uint32_t OGLGraphics::UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& elems) {
  uint32_t VAO, VBO, EBO;
  glGenBuffers(1, &VBO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elems.size() * sizeof(uint32_t), &elems[0], GL_STATIC_DRAW);

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
void OGLGraphics::DeleteMesh(const uint32_t& VAO) {
  glDeleteBuffers(1, &VAO);
}

uint32_t OGLGraphics::Upload2DTex(const unsigned char* tex_data, int width, int height) {
  unsigned int out_texID = 0;
  glGenTextures(1, &out_texID);
  glBindTexture(GL_TEXTURE_2D, out_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //try: https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
  //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  return out_texID;
}

uint32_t OGLGraphics::UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height) {
  unsigned int out_texID;
  glGenTextures(1, &out_texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, out_texID);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (auto i = 0; i < 6; ++i) {
    if (tex_data[i]) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data[i]);
    }
  }
  return out_texID;
}

void OGLGraphics::DeleteTex(const uint32_t& id) {
  glDeleteTextures(1, &id);
}

void OGLGraphics::SetMSAA(const bool enabled) {
  glEnable(GL_MULTISAMPLE);
}

void SetBlend(const bool enabled) {
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

}  // end namespace OGLGraphics
}  // end namespace AA