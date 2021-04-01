#include <sstream>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../Renderer/SceneLoader.h"
#include "../Renderer/OpenGL/OGLGraphics.h"
#include "../Renderer/Vertex.h"
#include "../Settings/Settings.h"
#include <stb_image.h>
#include <iostream>

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

int SceneLoader::LoadGameObjectFromFile(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path)
{
	Assimp::Importer importer;
	int post_processing_flags = 0;
	//post processing -> http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	post_processing_flags |= aiProcess_JoinIdenticalVertices | aiProcess_Triangulate;// |
#ifdef D3D
		aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs |
#endif
		//aiProcess_PreTransformVertices |
		//aiProcess_CalcTangentSpace |
		//aiProcess_GenSmoothNormals |
		//aiProcess_Triangulate |
		//aiProcess_FixInfacingNormals |
		//aiProcess_FindInvalidData |
		//aiProcess_ValidateDataStructure;

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
	const aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
	std::unordered_map<uint32_t, std::string> all_loaded_textures;

	// get the albedo (diffuse) textures
	std::unordered_map<uint32_t, std::string> albedo_textures;
	if (loadMaterialTextures(scene, ai_material, aiTextureType_DIFFUSE, "Albedo", albedo_textures) == 0)
	{
		for (auto& a_tex : albedo_textures)
		{
			all_loaded_textures.insert(all_loaded_textures.end(), a_tex);
			std::cout << "found&loaded Albedo texture\n";
		}
	}

	int shading_model;
	ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_model);
	std::cout << "shading model: " << shading_model << '\n';
	ai_real shininess = .1f;

	if (shading_model == aiShadingMode_Phong)
	{
		std::cout << "shading model is phong\n";

		std::unordered_map<uint32_t, std::string> specular_textures;
		if (loadMaterialTextures(scene, ai_material, aiTextureType_SPECULAR, "Specular", specular_textures) == 0)
		{
			for (auto& s_tex : specular_textures)
			{
				all_loaded_textures.insert(all_loaded_textures.end(), s_tex);
				std::cout << "found&loaded Specular texture\n";
			}
		}

		if (!ai_material->Get(AI_MATKEY_SHININESS, shininess))
		{
			// set shininess to a default if it failed
			shininess = 6.1f;
			std::cout << "shininess not found, shininess defaulted to 6.1f\n";
			//assimpDoesntFindShininessLetsDoItInstead(path, &shininess);
		}
	}

	//aiColor4D spec_color;
	//if (AI_SUCCESS != aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &spec_color))
	//{
	//	// set spec_color to a default if it failed to find
	//	spec_color = aiColor4D(0.1f, 0.1f, 0.1f, 1.f);
	//	std::cout << "specular not found, defaulted to .1f\n";
	//}

	uint32_t vao = 0;
	switch (Settings::Get()->GetOptions().renderer)
	{
	case RenderingFramework::OPENGL:
		vao = OGLGraphics::UploadMesh(loaded_vertices, loadedElements);
		break;
	}
	return MeshDrawInfo(vao, (uint32_t)loadedElements.size(), all_loaded_textures, shininess, SceneLoader::aiMat4_to_glmMat4(*trans));
}

/// <summary>
/// handles loading a texture or reusing an already loaded one. increments mTexturesLoaded textures if they were reused
/// </summary>
/// <param name="scn">scene being loaded</param>
/// <param name="mat">material being loaded</param>
/// <param name="type">type of the material to look for</param>
/// <param name="typeName">type of texture (albedo, etc)</param>
/// <param name="out_texInfo">populate if successful</param>
/// <returns>0 for success, all negative returns are error</returns>
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
		// read texture from memory if above was successful
		if (ai_embedded_texture)
		{
			std::string embedded_filename = ai_embedded_texture->mFilename.C_Str();
			for (auto& available_texture : mLoadedTextures)
			{
				// if texture path already loaded, just give the mesh the details
				if (available_texture.path.data() == embedded_filename)
				{
					out_texInfo.insert(out_texInfo.end(), { available_texture.accessId, available_texture.type });
					available_texture.ref_count++;
					//return 0;  // success
					break;
				}
			}
			// not already loaded, ok lets load it
			TextureInfo a_new_texture_info;
			bool compressed = false;
			if (ai_embedded_texture->mHeight == 0)
			{
				compressed = true;
			}
			int width, height, nrComponents;
#ifdef D3D
			stbi_set_flip_vertically_on_load(0);
#else
			stbi_set_flip_vertically_on_load(1);
#endif
			int texture_size = ai_embedded_texture->mWidth * std::max(ai_embedded_texture->mHeight, 1u);
			unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(ai_embedded_texture->pcData), texture_size, &width, &height, &nrComponents, STBI_rgb);
			if (data)
			{
				switch (Settings::Get()->GetOptions().renderer)
				{
				case RenderingFramework::OPENGL:
					a_new_texture_info.accessId = OGLGraphics::Upload2DTex(data, width, height);
					if (a_new_texture_info.accessId != 0)
					{
						// add the new one to our list of loaded textures
						a_new_texture_info.path = embedded_filename;
						a_new_texture_info.type = typeName;
						// update our list of loaded textures
						mLoadedTextures.push_front(a_new_texture_info);
						// to return for draw info on this current mesh
						out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
						//return 0; // success!
					}
					break;
				}
			}
			//return -1; // failed to get embedded texture
		}
		else  //regular file, check if it exists and read it
		{
			std::vector<std::string> potential_paths;
			// the 3 paths to try
			// 1. the literal given path (will probably fail)
			potential_paths.emplace_back(aiTmpStr.C_Str());
			// 2. the path based on where the model was loaded from (might work)
			std::string literal_path = mModelDir + aiTmpStr.C_Str();
			potential_paths.emplace_back(literal_path);
			// 3. the last part of the given path (after '/' or '\\') appended to the path based on were the model was loaded from
			std::string from_model_path = mModelDir + literal_path.substr(literal_path.find_last_of("/\\") + 1);  // all the way to the end
			potential_paths.emplace_back(from_model_path);
			// routine to see if we already have this texture loaded
			for (auto& available_texture : mLoadedTextures)
			{
				for (const auto& a_path : potential_paths)
				{
					if (available_texture.path.data() == a_path)
					{
						// texture already loaded, just give the mesh the details
						out_texInfo.insert(out_texInfo.end(), { available_texture.accessId, available_texture.type });
						available_texture.ref_count++;
						return 0;  // success
					}
				}
			}
			// wasn't already loaded, lets try to load it
			TextureInfo a_new_texture_info;
			int width, height, nrComponents;
#ifdef D3D
			stbi_set_flip_vertically_on_load(0);
#else
			stbi_set_flip_vertically_on_load(1);
#endif
			// try 1
			unsigned char* data = nullptr;
			for (const auto& a_path : potential_paths)
			{
				data = stbi_load(a_path.c_str(), &width, &height, &nrComponents, STBI_rgb);
				if (data)
				{
					switch (Settings::Get()->GetOptions().renderer)
					{
					case RenderingFramework::OPENGL:
						a_new_texture_info.accessId = OGLGraphics::Upload2DTex(data, width, height);
						break;
					}
					if (a_new_texture_info.accessId != 0)
					{
						// add the new one to our list of loaded textures
						a_new_texture_info.path = a_path;
						a_new_texture_info.type = typeName;
						mLoadedTextures.push_front(a_new_texture_info);
						// to return for draw info on this current mesh
						out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
						stbi_image_free(data);
						//return 0; // success!
						break;
					}
				}
				stbi_image_free(data);
			}
			//return -1; // failed to load new texture - all pathing failed
			break;
		}
	}

	// went through the above loop without error, mLoadedTextures & out_texInfo should be updated
	return 0;
}

void SceneLoader::UnloadGameObject(const std::vector<MeshDrawInfo>& toUnload)
{
	for (const auto& meshIt : toUnload)
	{
		switch (Settings::Get()->GetOptions().renderer)
		{
		case RenderingFramework::OPENGL:
			OGLGraphics::DeleteMesh(meshIt.vao);
			for (const auto& texIt : meshIt.textureDrawIds)
			{
				for (auto loaded_tex = mLoadedTextures.begin(); loaded_tex != mLoadedTextures.end(); loaded_tex++)
				{
					if (texIt.first == loaded_tex->accessId)
					{
						loaded_tex->ref_count--;
						if (loaded_tex->ref_count == 0)
						{
							OGLGraphics::DeleteTex(loaded_tex->accessId);
						}
					}
				}

				// sync textures
				mLoadedTextures.remove_if([](const TextureInfo& ti) {
					if (ti.ref_count == 0)
						return true;
					else
						return false;
					});
			}
			break;
		}
	}
}
} // end namespace AA
