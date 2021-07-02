#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "OGLShader.h"
#include "../../../include/AncientArcher/ShaderType.h"
#include "../../../include/AncientArcher/Types.h"
#include "../MeshDrawInfo.h"
#include "../../Scene/Vertex.h"

namespace AA {
extern OGLShader* mLitShader;
extern OGLShader* mDiffShader;
namespace OGLGraphics {
void SetSamplerCube(int which, const int& cubetexID);
void SetTexture(int which, const int& textureID);
void RenderSkybox(const int& vao, const int& count);
void Render(const std::vector<MeshDrawInfo>& meshes, const glm::mat4& translationMatrix, SHADERTYPE shadertype);
void RenderStrip(const int& vao, const int& count);
void SetViewportSize(int x, int y, int w, int h);
void SetViewportClearColor(glm::vec3 color) noexcept;
void ClearScreen() noexcept;
u32 UploadMesh(const float *points, const int num_points);
u32 UploadMesh(const std::vector<Vertex>& verts, const std::vector<u32>& elems);
u32 UploadMesh(const float *points, const int num_points, const u32 *indices, const int ind_count);
u32 Upload2DVerts(const std::vector<vec2>& points);
u32 Upload2DVerts(const std::vector<float>& points);
void DeleteMesh(const u32& VAO);
u32 Upload2DTex(const unsigned char* tex_data, int width, int height, bool hasAlpha = true);
u32 UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height, bool hasAlpha = true);
void DeleteTex(const u32& id);
void SetMSAA(const bool enabled);
void SetBlend(const bool enabled);
void SetCullFace(const bool enabled);
};
}  // end namespace AA