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
class OGLGraphics
{
	friend class GameObject;
	friend class Display;
	friend class SceneLoader;
	friend class Skybox;
private:
	static void Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, const OGLShader& modelShader);
	static void SetViewportSize(int x, int y, int w, int h);
	static void SetViewportClearColor(glm::vec3 color) noexcept;
	static void ClearScreen() noexcept;
	//static uint32_t UploadMesh(std::vector<Vertex> verts, std::vector<uint32_t> elems);
	static uint32_t UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& elems);
	static void DeleteMesh(const uint32_t&  VAO);
	static uint32_t Upload2DTex(const unsigned char* tex_data, int width, int height);
	static uint32_t UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height);
	static void DeleteTex(const uint32_t& id);
	static void SetMSAA(const bool enabled);
};
}  // end namespace AA