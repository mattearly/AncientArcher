#include "../../include/Scene/TexLoader.h"
#include <memory>
#include <stdexcept>
#include <glad/glad.h>
#include <stb_image.h>
#ifdef _DEBUG
#include <iostream>
#endif
namespace AA
{
TexLoader* TexLoader::getInstance()
{
	static TexLoader* tl = new TexLoader();
	return tl;
	//static std::unique_ptr<TexLoader> staticTexLoader = std::make_unique<TexLoader>();
	//return staticTexLoader.get();
}

/**
 * This code loads in a cube map texture.
 * @param[in] files       to the textures
 * @return    textureID   id to reference the loaded texture
 */
unsigned int TexLoader::loadCubeTexture(const std::vector<std::string>& files)
{
	if (files.size() != 6)
	{
		throw std::runtime_error("Not enough files for cube map. A cube map needs to include 6 textures.");
	}
	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	stbi_set_flip_vertically_on_load(0); // tell stb_image.h to not flip loaded texture's on the y-axis.
	int width, height, nrChannel;
	std::size_t size = files.size();
	for (auto i = 0; i < size; ++i)
	{
		unsigned char* data = stbi_load(files[i].c_str(), &width, &height, &nrChannel, STBI_rgb);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);
			throw std::runtime_error("A cubemap texture was not able to be loaded -- check path.");
			return 0;
		}
	}

	return texID;
}

unsigned int TexLoader::textureFromFile(const char* filepath, bool gamma)
{
	unsigned int out_texID = 0;

	int width, height, nrComponents;

#ifdef D3D
	stbi_set_flip_vertically_on_load(0);
#else
	stbi_set_flip_vertically_on_load(1);
#endif

	//unsigned char* data = stbi_load(filepath, &width, &height, &nrComponents, 0);
	unsigned char* data = stbi_load(filepath, &width, &height, &nrComponents, STBI_rgb);
	if (data)
	{
		//GLenum format{};
		//if (nrComponents == 1)
		//{
		//	format = GL_RED;
		//}
		//else if (nrComponents == 3)
		//{
		//	format = GL_RGB;
		//}
		//else if (nrComponents == 4)
		//{
		//	format = GL_RGBA;
		//}

		glGenTextures(1, &out_texID);
		glBindTexture(GL_TEXTURE_2D, out_texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//try: https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
#ifdef _DEBUG
		std::cout << "::TEXLOADER: SUCCESS at path: " << filepath << std::endl;
#endif
	}
	else
	{
#ifdef _DEBUG
		std::cout << "::TEXLOADER: failed to load at path: " << filepath << std::endl;
#endif
		stbi_image_free(data);
	}

	return out_texID;
}
/// <summary>
/// for use when the texture is already in memory (such as with fbx embedded textures)
/// </summary>
/// <param name="tex">the embedded texture</param>
/// <returns>access opengl id</returns>
unsigned int TexLoader::textureFromData(const aiTexture* tex)
{
	unsigned int out_texID = 0;

	//if (tex->mWidth == 0)
	//{
	//	std::cout << "width says compressed data texture\n";
	//	return out_texID;
	//}
	bool compressed = false;
	if (tex->mHeight == 0)
	{
#ifdef _DEBUG
		std::cout << "height says compressed data texture\n";
#endif
		//return out_texID;
		compressed = true;
	}

	int width, height, nrComponents;

	int texture_size = tex->mWidth * std::max(tex->mHeight, 1u);

	unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(tex->pcData), texture_size, &width, &height, &nrComponents, STBI_rgb);

	//size_t texture_size = static_cast<size_t>(tex->mWidth * std::max(tex->mHeight, 1u));

	//std::vector<uint8_t> tex_data;
	//tex_data.resize(texture_size);
	//memcpy(&tex_data[0], (char*)tex->pcData, texture_size);

	if (data)
	{
#ifdef _DEBUG
		std::cout << "loading from tex data to opengl...\n";
#endif
		glGenTextures(1, &out_texID);
		glBindTexture(GL_TEXTURE_2D, out_texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//try: https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	//char* img;
	//int w, h, n;
	//extern char _binary_font_png_start, _binary_font_png_end;
	//img = (char*)stbi_load_from_memory((unsigned char*)&_binary_font_png_start, &_binary_font_png_end - &_binary_font_png_start, &w, &h, &n, 4);

	return out_texID;
}
TexLoader::TexLoader()
{
}
} // end namespace AA