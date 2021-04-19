#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "OGLShader.h"
#include "../../../include/AncientArcher/ShaderType.h"
#include "../../../include/AncientArcher/Types.h"
#include "../MeshDrawInfo.h"
#include "../Vertex.h"

namespace AA {
extern OGLShader* mLitShader;
extern OGLShader* mDiffShader;
namespace OGLGraphics {
void Render(const std::vector<MeshDrawInfo>& meshes, const glm::mat4& translationMatrix, SHADERTYPE shadertype);
void RenderStrip(const int& vao, const int& count);
void SetViewportSize(int x, int y, int w, int h);
void SetViewportClearColor(glm::vec3 color) noexcept;
void ClearScreen() noexcept;
uint32_t UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& elems);
u32 Upload2DVerts(const std::vector<vec2>& points);
u32 Upload2DVerts(const std::vector<float>& points);
void DeleteMesh(const uint32_t& VAO);
uint32_t Upload2DTex(const unsigned char* tex_data, int width, int height);
uint32_t UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height);
void DeleteTex(const uint32_t& id);
void SetMSAA(const bool enabled);
void SetBlend(const bool enabled);
};
}  // end namespace AA