#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "OGLShader.h"
#include "../../../include/AncientArcher/ShaderType.h"
#include "../MeshDrawInfo.h"
#include "../../Base/Types.h"
#include "../Vertex.h"

namespace AA
{
extern OGLShader* mLitShader;
extern OGLShader* mDiffShader;
extern OGLShader* mInterfaceShader;
namespace OGLGraphics
{
//void Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, bool lit);
//void Render(const std::vector<MeshDrawInfo>& meshes, const glm::mat4& translationMatrix, bool lit);
void Render(const std::vector<MeshDrawInfo>& meshes, const glm::mat4& translationMatrix, SHADERTYPE shadertype);
//void Render(const std::vector<MeshDrawInfo>& meshes, bool lit);
void SetViewportSize(int x, int y, int w, int h);
void SetViewportClearColor(glm::vec3 color) noexcept;
void ClearScreen() noexcept;
uint32_t UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& elems);
u32 UploadVerts(const std::vector<vec3>& points, const std::vector<u32>& elems);
void DeleteMesh(const uint32_t& VAO);
uint32_t Upload2DTex(const unsigned char* tex_data, int width, int height);
uint32_t UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height);
void DeleteTex(const uint32_t& id);
void SetMSAA(const bool enabled);
};
}  // end namespace AA