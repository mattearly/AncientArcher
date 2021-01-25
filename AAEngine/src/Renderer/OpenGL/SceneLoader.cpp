/*
SceneLoader.cpp
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
#include <sstream>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../../../include/Renderer/OpenGL/SceneLoader.h"
#include "../../../include/Renderer/Vertex.h"
#include "../../../include/Renderer/OpenGL/TexLoader.h"

namespace AA
{
SceneLoader* SceneLoader::Get() {
	static SceneLoader* sl = new SceneLoader();
	return sl;
}

SceneLoader::SceneLoader()
{
	mModelDir = "";
	mModelFileName = "";
	mModelFileName = "";
	mTexturesLoaded.clear();
}

glm::mat4 SceneLoader::aiMat4_to_glmMat4(const aiMatrix4x4& inMat)
{
	glm::mat4 outMat{};
	outMat[0][0] = inMat.a1;
	outMat[1][0] = inMat.b1;
	outMat[2][0] = inMat.c1;
	outMat[3][0] = inMat.d1;
	outMat[0][1] = inMat.a2;
	outMat[1][1] = inMat.b2;
	outMat[2][1] = inMat.c2;
	outMat[3][1] = inMat.d2;
	outMat[0][2] = inMat.a3;
	outMat[1][2] = inMat.b3;
	outMat[2][2] = inMat.c3;
	outMat[3][2] = inMat.d3;
	outMat[0][3] = inMat.a4;
	outMat[1][3] = inMat.b4;
	outMat[2][3] = inMat.c4;
	outMat[3][3] = inMat.d4;
	//glm::transpose(outMat);
	return outMat;
}

glm::vec3 SceneLoader::aiVec3_to_glmVec3(const aiVector3D& inVec) noexcept
{
	glm::vec3 outVec{};
	outVec.x = inVec.x;
	outVec.y = inVec.y;
	outVec.z = inVec.z;
	return outVec;
}

glm::vec4 SceneLoader::aiColor4_to_glmVec4(const aiColor4D& inVec) noexcept
{
	glm::vec4 outVec{};
	outVec.x = inVec.r;
	outVec.y = inVec.g;
	outVec.z = inVec.b;
	outVec.w = inVec.a;

	return outVec;
}

glm::quat SceneLoader::aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept
{
	glm::quat outQuat{};
	outQuat.w = inQuat.w;
	outQuat.x = inQuat.x;
	outQuat.y = inQuat.y;
	outQuat.z = inQuat.z;
	return outQuat;
}

int SceneLoader::loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path)
{
	Assimp::Importer importer;
	int post_processing_flags = 0;
	//post processing -> http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	post_processing_flags |= aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |
#ifdef D3D
		aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs |
#endif
		aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure;

	const aiScene* scene = importer.ReadFile(path, post_processing_flags);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return -1;  // failed to load scene
	}

	std::size_t the_last_slash = path.find_last_of("/\\") + 1;
	std::size_t the_last_dot = path.find_last_of(".");

	mModelDir = path.substr(0, the_last_slash);  // path to filename's dir

	mModelFileExtension = path.substr(
		static_cast<std::basic_string<char,
		std::char_traits<char>,
		std::allocator<char>>::size_type>(the_last_dot) + 1);  // get the file extension (type of file)

	mModelFileName = path.substr(
		the_last_slash,
		static_cast<std::basic_string<char,
		std::char_traits<char>,
		std::allocator<char>>::size_type>(the_last_dot) - the_last_slash);  // get the name of the file

	processNode(scene->mRootNode, scene, out_MeshInfo);

	return 0;
}

void SceneLoader::unloadGameObject(const std::vector<MeshDrawInfo>& toUnload)
{
	for (const auto& meshIt : toUnload)
	{
		glDeleteBuffers(1, &meshIt.vao);
		for (const auto& texIt : meshIt.textureDrawIds)
		{
			glDeleteTextures(1, &texIt.first);
		}
	}
}

void SceneLoader::processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_MeshInfo)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		out_MeshInfo.push_back(processMesh(mesh, scene, &node->mTransformation));
	}

	for (uint32_t i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene, out_MeshInfo);
	}
}

///
/// Get all the vertex Data for the incoming mesh and scene
///
MeshDrawInfo SceneLoader::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans)
{
	std::vector<Vertex> loaded_vertices;
	uint32_t num_of_vertices_on_mesh = mesh->mNumVertices;

	// get the vertices
	for (uint32_t i = 0; i < num_of_vertices_on_mesh; ++i)
	{
		const glm::vec3 temp_position = SceneLoader::aiVec3_to_glmVec3(mesh->mVertices[i]);

		glm::vec2 temp_tex_coords(0);
		if (mesh->mTextureCoords[0] != nullptr)
		{
			temp_tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		const glm::vec3 temp_norm = SceneLoader::aiVec3_to_glmVec3(mesh->mNormals[i]);

		loaded_vertices.emplace_back(Vertex(temp_position, temp_tex_coords, temp_norm));
	}

	// get the indices to draw triangle faces with
	std::vector<uint32_t> loadedElements;
	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			loadedElements.push_back(face.mIndices[j]);
		}
	}

	// get the materials
	const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::unordered_map<uint32_t, std::string> all_loaded_textures;

	std::unordered_map<uint32_t, std::string> albedo_textures;

	// if succeeds in loading texture add it to loaded texutres
	if (loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, "Albedo", albedo_textures) == 0)
	{
		for (auto& newtexture : albedo_textures)
			all_loaded_textures.insert(all_loaded_textures.end(), newtexture);
	}

	uint32_t VAO, VBO, EBO;

	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, loaded_vertices.size() * sizeof(Vertex), &loaded_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedElements.size() * sizeof(uint32_t), &loadedElements[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return MeshDrawInfo(VAO, (uint32_t)loadedElements.size(), all_loaded_textures, SceneLoader::aiMat4_to_glmMat4(*trans));
}

int SceneLoader::loadMaterialTextures(const aiScene* scn, const aiMaterial* mat, aiTextureType type, std::string typeName, std::unordered_map<uint32_t, std::string>& out_texInfo)
{
	for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString aiTmpStr;
		auto tex_success = mat->GetTexture(type, i, &aiTmpStr);

		switch (tex_success)
		{
		case aiReturn_SUCCESS:
			break;
		case aiReturn_FAILURE:
			return -1;
			break;
		case aiReturn_OUTOFMEMORY:
			return -2;
			break;
		}


		// try from embedded
		const aiTexture* ai_embedded_texture = scn->GetEmbeddedTexture(aiTmpStr.C_Str());
		if (ai_embedded_texture)
		{
			//returned pointer is not null, read texture from memory
			std::string embedded_filename = ai_embedded_texture->mFilename.C_Str();
			for (uint32_t j = 0; j < mTexturesLoaded.size(); ++j)
			{
				for (const auto& p : mTexturesLoaded)
				{
					if (p.path.data() == embedded_filename)
					{
						// texture already loaded, just give the mesh the details
						out_texInfo.insert(out_texInfo.end(), { p.accessId, p.type });
						return 0;  // success
					}
				}
			}

			// ok, load it
			TextureInfo a_new_texture_info;

			a_new_texture_info.accessId = TexLoader::textureFromData(ai_embedded_texture);
			if (a_new_texture_info.accessId != 0)
			{
				// add the new one to our list of loaded textures
				a_new_texture_info.path = embedded_filename;
				a_new_texture_info.type = typeName;
				mTexturesLoaded.push_back(a_new_texture_info);

				// to return for draw info on this current mesh
				out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
				return 0; // success!
			}

			return -1; // failed to get embedded texture

		}
		//regular file, check if it exists and read it
		else
		{

			// try 3 paths
			// 1. the literal given path (will probably fail)
			// 2. the path based on where the model was loaded from (might work)
			// 3. the last part of the given path (after '/' or '\\') appended to the path based on were the model was loaded from
			std::string tex_path1_literal = aiTmpStr.C_Str();
			std::string tex_path2_loadedFromFullAppend = mModelDir + tex_path1_literal;
			std::string tex_path3_loadedFromEndAppend = mModelDir + tex_path1_literal.substr(tex_path1_literal.find_last_of("/\\") + 1);  // all the way to the end
			// routine to see if we already have this texture loaded
			bool alreadyLoaded = false;
			for (uint32_t j = 0; j < mTexturesLoaded.size(); ++j)
			{
				for (const auto& p : mTexturesLoaded)
				{
					if (p.path.data() == tex_path1_literal || p.path.data() == tex_path2_loadedFromFullAppend || p.path.data() == tex_path3_loadedFromEndAppend)
					{
						// texture already loaded, just give the mesh the details
						out_texInfo.insert(out_texInfo.end(), { p.accessId, p.type });
						return 0;  // success
					}
				}
			}

			TextureInfo a_new_texture_info;

			a_new_texture_info.accessId = TexLoader::textureFromFile((tex_path1_literal).c_str());
			if (a_new_texture_info.accessId != 0)
			{
				// add the new one to our list of loaded textures
				a_new_texture_info.path = tex_path1_literal;
				a_new_texture_info.type = typeName;
				mTexturesLoaded.push_back(a_new_texture_info);

				// to return for draw info on this current mesh
				out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
				return 0; // success!
			}

			a_new_texture_info.accessId = TexLoader::textureFromFile(tex_path2_loadedFromFullAppend.c_str());
			if (a_new_texture_info.accessId != 0)
			{
				// add the new one to our list of loaded textures
				a_new_texture_info.path = tex_path2_loadedFromFullAppend;
				a_new_texture_info.type = typeName;
				mTexturesLoaded.push_back(a_new_texture_info);

				// to return for draw info on this current mesh
				out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
				return 0; // success!
			}

			a_new_texture_info.accessId = TexLoader::textureFromFile(tex_path3_loadedFromEndAppend.c_str());
			if (a_new_texture_info.accessId != 0)
			{
				// add the new one to our list of loaded textures
				a_new_texture_info.path = tex_path3_loadedFromEndAppend;
				a_new_texture_info.type = typeName;
				mTexturesLoaded.push_back(a_new_texture_info);

				// to return for draw info on this current mesh
				out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
				return 0; // success!
			}

			return -1; // failed to load new texture - all pathing failed
		}
	}

	return -1; // failed to return a success so probably a fail (we'll never get here but most static analysis give a warning if we don't have this here)
}
} // end namespace AA