#include "../include/SceneLoader.h"
#ifdef _DEBUG
#include <iostream>
#endif
#include <sstream>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../include/Vertex.h"
#include "../include/TexLoader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AA
{
SceneLoader* SceneLoader::getSceneLoader() {
	static SceneLoader* sl = new SceneLoader();
	sl->mLastDir = "";
	return sl;
}

SceneLoader::SceneLoader()
{
	mLastDir = "";
	mTexturesLoaded.clear();
}

glm::mat4 SceneLoader::aiMat4_to_glmMat4(const aiMatrix4x4& inMat)
{
	glm::mat4 outMat;
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
	glm::vec3 outVec;
	outVec.x = inVec.x;
	outVec.y = inVec.y;
	outVec.z = inVec.z;
	return outVec;
}

glm::vec4 SceneLoader::aiColor4_to_glmVec4(const aiColor4D& inVec) noexcept
{
	glm::vec4 outVec;

	outVec.x = inVec.r;
	outVec.y = inVec.g;
	outVec.z = inVec.b;
	outVec.w = inVec.a;

	return outVec;
}

glm::quat SceneLoader::aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept
{
	glm::quat outQuat;
	outQuat.w = inQuat.w;
	outQuat.x = inQuat.x;
	outQuat.y = inQuat.y;
	outQuat.z = inQuat.z;
	return outQuat;
}

int SceneLoader::loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path)
{
#ifdef _DEBUG
	std::cout << "------\n";
	std::cout << "LOADING GAME OBJECT: path : " << path << "\n";
#endif _DEBUG
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
#ifdef _DEBUG
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
#endif
		return -1;  // failed to load scene
	}

	int the_last_slash = path.find_last_of("/\\") + 1;
	int the_last_dot = path.find_last_of(".");

	mLastDir = path.substr(0, the_last_slash);  // path to filename's dir

	mLastFileExtension = path.substr(the_last_dot + 1);  // get the file extension (type of file)

	mLastFileName = path.substr(the_last_slash, the_last_dot - the_last_slash);  // get the name of the file

#ifdef _DEBUG
	std::cout << "last dir = " << mLastDir << '\n';
	std::cout << "last file extension = " << mLastFileExtension << '\n';
	std::cout << "last filename = " << mLastFileName << '\n';
#endif

	//std::string getBasePath(const std::string & path)
	//{
	//  size_t pos = path.find_last_of("\\/");
	//  return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
	//}

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

	std::unordered_map<uint32_t, std::string> loadedTextures;

	std::unordered_map<uint32_t, std::string> textureUnitMaps;

	// if succeeds in loading texture add it to loaded texutres
	if (loadMaterialTextures(material, aiTextureType_DIFFUSE, "Albedo", textureUnitMaps) == 0)
	{
		//loadedTextures.insert(loadedTextures.end(), textureUnitMaps.begin(), textureUnitMaps.end());

		for (auto newtexture : textureUnitMaps)
			loadedTextures.insert(loadedTextures.end(), newtexture);
	}
#ifdef _DEBUG
	else
	{
		std::cout << "failed to load aiTextureType_DIFFUSE\n";
	}
#endif

	//std::vector<TextureInfo> specMaps;
	//loadMaterialTextures(material, aiTextureType_SPECULAR, "specular", specMaps);
	//loadedTextures.insert(loadedTextures.end(), specMaps.begin(), specMaps.end());

	//std::vector<TextureInfo> normMaps;
	//loadMaterialTextures(material, aiTextureType_HEIGHT, "normal", normMaps);
	//loadedTextures.insert(loadedTextures.end(), normMaps.begin(), normMaps.end());

	//std::vector<TextureInfo> heightMaps;
	//loadMaterialTextures(material, aiTextureType_AMBIENT, "height", heightMaps);
	//loadedTextures.insert(loadedTextures.end(), heightMaps.begin(), heightMaps.end());

	//std::vector<TextureInfo> colorMaps;
	//loadMaterialTextures(material, aiTextureType_BASE_COLOR, "base color", colorMaps);
	//loadedTextures.insert(loadedTextures.end(), colorMaps.begin(), colorMaps.end());

	//float shine(1);
	//if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shine))
	//{
	//	shine = .5f;
	//	std::cout << "didn't get material shininess, set to .5f\n";
	//}
	//else  // success
	//{
	//	std::cout << "material shininess found! set to: " << shine << '\n';
	//}

	//aiColor4D spec;
	//glm::vec4 specular(1);
	//if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec))
	//{
	//	specular = glm::vec4(1.f);
	//	std::cout << "didn't get material specular, set to 1,1,1,1\n";  //debug
	//}
	//else  // success
	//{
	//	specular = SceneLoader::aiColor4_to_glmVec4(spec);
	//	std::cout << "material specular found! set to "
	//		<< specular.r << ','
	//		<< specular.g << ','
	//		<< specular.b << ','
	//		<< specular.a << '\n';  // debug
	//}

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

	return MeshDrawInfo(VAO, (uint32_t)loadedElements.size(), loadedTextures, SceneLoader::aiMat4_to_glmMat4(*trans));
}

int SceneLoader::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::unordered_map<uint32_t, std::string>& out_texInfo)
{
	for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString aiTmpStr;
		auto tex_success = mat->GetTexture(type, i, &aiTmpStr);

		switch (tex_success)
		{
		case aiReturn_SUCCESS:
#ifdef _DEBUG
			std::cout << "Attempting Texture on given path: " << aiTmpStr.C_Str() << '\n';
#endif
			break;
		case aiReturn_FAILURE:
#ifdef _DEBUG
			std::cout << "failure getting texture on material tex num " << i << ' ' << aiTmpStr.C_Str() << '\n';
#endif
			return -1;
			break;
		case aiReturn_OUTOFMEMORY:

#ifdef _DEBUG
			std::cout << "oom getting texture on material tex num " << i << ' ' << aiTmpStr.C_Str() << '\n';
#endif
			return -1;
			break;
		}

		// try 3 paths
		// 1. the literal given path (will probably fail)
		// 2. the path based on where the model was loaded from (might work)
		// 3. the last part of the given path (after '/' or '\\') appended to the path based on were the model was loaded from
		std::string tex_path1_literal = aiTmpStr.C_Str();
		std::string tex_path2_loadedFromFullAppend = mLastDir + tex_path1_literal;
		std::string tex_path3_loadedFromEndAppend = mLastDir + tex_path1_literal.substr(tex_path1_literal.find_last_of("/\\") + 1);  // all the way to the end
		std::string tex_path4_fbxpacked = mLastDir + mLastFileName + ".fbm";
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
#ifdef _DEBUG
					std::cout << "Texture [" << p.path.data() << "] already loaded, using it.\n";
#endif
					return 0;  // success
				}
			}
		}

		if (!alreadyLoaded)
		{
			TextureInfo a_new_texture_info;

#ifdef _DEBUG
			std::cout << " - TexLoad try 1 (given path): " << tex_path1_literal << '\n';
#endif
			a_new_texture_info.accessId = TexLoader::getInstance()->textureFromFile((tex_path1_literal).c_str());
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
#ifdef _DEBUG
			std::cout << " - TexLoad try 2 (full append): " << tex_path2_loadedFromFullAppend << '\n';
#endif
			a_new_texture_info.accessId = TexLoader::getInstance()->textureFromFile(tex_path2_loadedFromFullAppend.c_str());
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
#ifdef _DEBUG
			std::cout << " - TexLoad try 3 (end append): " << tex_path3_loadedFromEndAppend << '\n';
#endif
			a_new_texture_info.accessId = TexLoader::getInstance()->textureFromFile(tex_path3_loadedFromEndAppend.c_str());
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
#ifdef _DEBUG
			std::cout << " - TexLoad try 4 (fbx packed): " << tex_path4_fbxpacked << '\n';
#endif
			a_new_texture_info.accessId = TexLoader::getInstance()->textureFromFile(tex_path4_fbxpacked.c_str());
			if (a_new_texture_info.accessId != 0)
			{
				// add the new one to our list of loaded textures
				a_new_texture_info.path = tex_path4_fbxpacked;
				a_new_texture_info.type = typeName;
				mTexturesLoaded.push_back(a_new_texture_info);

				// to return for draw info on this current mesh
				out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
				return 0; // success!
			}

			return -1; // failed to load new texture - all pathing failed
		}
	}

	return 0; // success (we'll never get here but most static analysis give a warning if we don't have this here)
}
} // end namespace AA