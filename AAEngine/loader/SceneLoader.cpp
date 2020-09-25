#include "SceneLoader.h"
#include <iostream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Vertex.h"
#include "TexLoader.h"
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

int SceneLoader::loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path, bool pp_triangulate)
{
	Assimp::Importer importer;
	int post_processing_flags = 0;
	post_processing_flags |= aiProcess_JoinIdenticalVertices;
	//post_processing_flags |= aiProcess_FlipUVs;
	if (pp_triangulate)
	{
		post_processing_flags |= aiProcess_Triangulate;
	}
	const aiScene* scene = importer.ReadFile(path, post_processing_flags);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
		return -1;  // failed to load scene
	}

	mLastDir = path.substr(0, path.find_last_of("/\\") + 1);  // get the beginning of the filename before the last / or \\

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
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		out_MeshInfo.push_back(processMesh(mesh, scene, &node->mTransformation));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene, out_MeshInfo);
	}
}

///
/// Get all the vertex Data for the incoming mesh and scene
/// todo: fix translation for multi meshes (currently they all center which is wrong)
///
MeshDrawInfo SceneLoader::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans)
{
	std::vector<Vertex> loaded_vertices;
	unsigned int num_of_vertices_on_mesh = mesh->mNumVertices;

	// get the vertices
	for (unsigned int i = 0; i < num_of_vertices_on_mesh; ++i)
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
	std::vector<unsigned int> loadedElements;
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			loadedElements.push_back(face.mIndices[j]);
		}
	}

	// get the materials
	const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<TextureInfo> loadedTextures;

	std::vector<TextureInfo> textureUnitMaps;

	if (loadMaterialTextures(material, aiTextureType_DIFFUSE, "Albedo", textureUnitMaps) == 0) // if succeeds in loading texture add it to loaded texutres
	{
		loadedTextures.insert(loadedTextures.end(), textureUnitMaps.begin(), textureUnitMaps.end());
	}
	else
	{
		std::cout << "failed to load aiTextureType_DIFFUSE\n";
	}

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

	unsigned int VAO, VBO, EBO;

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedElements.size() * sizeof(unsigned int), &loadedElements[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return MeshDrawInfo(VAO, (unsigned int)loadedElements.size(), loadedTextures, SceneLoader::aiMat4_to_glmMat4(*trans));
}

int SceneLoader::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureInfo>& out_texInfo)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString tmpstr;
		auto tex_success = mat->GetTexture(type, i, &tmpstr);

		switch (tex_success)
		{
		case aiReturn_SUCCESS:
			std::cout << "success getting texture on material file " << i << ' ' << tmpstr.C_Str() << '\n';
			break;
		case aiReturn_FAILURE:
			std::cout << "failure getting texture on material file " << i << ' ' << tmpstr.C_Str() << '\n';
			break;
		case aiReturn_OUTOFMEMORY:
			std::cout << "oom getting texture on material file " << i << ' ' << tmpstr.C_Str() << '\n';
			break;
		}

		// routine to see if we already have this texture loaded
		bool alreadyLoaded = false;
		for (unsigned int j = 0; j < mTexturesLoaded.size(); ++j)
		{
			for (const auto& p : mTexturesLoaded)
			{
				if (p.path.data() == tmpstr.C_Str())
				{
					TextureInfo tmptexinfo;
					tmptexinfo.id = p.id;
					tmptexinfo.type = p.type;
					tmptexinfo.path = "";
					out_texInfo.push_back(tmptexinfo);
					alreadyLoaded = true;
					std::cout << "Texture [" << p.path.data() << "] already loaded.\n";
				}
			}
		}

		// wasn't loaded, load it trying last dir path
		if (!alreadyLoaded)
		{
			TextureInfo tmptex;
			//std::string tmpPath =/* mLastDir +*/ tmpstr.C_Str();
			std::string tmpPath = tmpstr.C_Str();

			std::cout << "attempting to load texture from " << mLastDir << tmpPath << '\n';

			tmptex.id = TexLoader::getInstance()->textureFromFile((mLastDir + tmpPath).c_str());
			if (tmptex.id != 0)
			{
				tmptex.path = tmpstr.C_Str();
				tmptex.type = typeName;
				out_texInfo.push_back(tmptex);
				mTexturesLoaded.push_back(tmptex);
			}
			else
			{
				// last dir path failed, try literal path instead
				std::cout << "attempting to load texture from " << tmpPath << '\n';

				tmptex.id = TexLoader::getInstance()->textureFromFile(tmpPath.c_str());
				if (tmptex.id != 0)
				{
					tmptex.path = tmpstr.C_Str();
					tmptex.type = typeName;
					out_texInfo.push_back(tmptex);
					mTexturesLoaded.push_back(tmptex);
				}
				else
				{
					return -1; // failed to load new texture - all pathing failed
				}
			}
		}
	}
	return 0;
}
} // end namespace AA