#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "OGLShader.h"
#include "../MeshDrawInfo.h"
#include "../../Scene/InstanceDetails.h"
#include "../Vertex.h"
namespace AA
{
extern OGLShader* mLitShader;
extern OGLShader* mDiffShader;
namespace OGLGraphics
{
	void Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, bool lit);
	void SetViewportSize(int x, int y, int w, int h);
	void SetViewportClearColor(glm::vec3 color) noexcept;
	void ClearScreen() noexcept;
	uint32_t UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& elems);
	void DeleteMesh(const uint32_t&  VAO);
	uint32_t Upload2DTex(const unsigned char* tex_data, int width, int height);
	uint32_t UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height);
	void DeleteTex(const uint32_t& id);
	void SetMSAA(const bool enabled);
};
}  // end namespace AA