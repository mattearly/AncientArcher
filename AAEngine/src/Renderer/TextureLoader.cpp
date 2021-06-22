#include "TextureLoader.h"

#include "../Settings/Settings.h"
#include "OpenGL/OGLGraphics.h"
#include "ModelLoader.h"

#include <stb_image.h>
#include <assimp/Importer.hpp>

namespace AA{
namespace TextureLoader{

std::forward_list<TextureInfo> mLoadedTextures;

u32 LoadTexture(const string& texture_path) {
  for (auto& a_tex : mLoadedTextures) {
    if (texture_path == a_tex.path.data()) {
      // texture already loaded, just give the mesh the details
      a_tex.ref_count++;
      return a_tex.accessId;  // the buck stops here
    }
  }

  auto rgb_type = STBI_rgb_alpha;

  TextureInfo a_new_texture_info;
  unsigned char* texture_data = nullptr;
  int width = 0, height = 0, nrComponents = 0;
  stbi_set_flip_vertically_on_load(1);
  texture_data = stbi_load(texture_path.c_str(), &width, &height, &nrComponents, rgb_type);
  if (texture_data) {
    switch (Settings::Get()->GetOptions().renderer) {
    case RenderingFramework::OPENGL:
      a_new_texture_info.accessId = OGLGraphics::Upload2DTex(texture_data, width, height);
      if (a_new_texture_info.accessId != 0) {
        // add the new one to our list of loaded textures
        a_new_texture_info.path = texture_path;
        a_new_texture_info.type = "Custom";  // todo: make this sync better
        a_new_texture_info.ref_count = 1;
        mLoadedTextures.push_front(a_new_texture_info);
        //std::cout << "loaded a new texture @ " << texture_path << "!\n";
      }
      break;
    default:
      throw("no renderer");
    }
  }
  stbi_image_free(texture_data);
  return a_new_texture_info.accessId;
}

int loadMaterialTextures(const aiScene* scn, const aiMaterial* mat, aiTextureType type, std::string typeName, std::unordered_map<u32, string>& out_texInfo) {
  u32 num_textures = mat->GetTextureCount(type);
  if (num_textures == 0) {
    return -3;
  }

  auto rgb_type = STBI_rgb_alpha;

  for (u32 i = 0; i < num_textures; ++i) {
    // make sure texture exists
    aiString aiTmpStr;
    auto tex_success = mat->GetTexture(type, i, &aiTmpStr);
    switch (tex_success) {
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
    if (ai_embedded_texture) {
      bool texture_has_loaded = false;
      std::string embedded_filename = ai_embedded_texture->mFilename.C_Str();
      for (auto& a_tex : mLoadedTextures) {
        // if texture path already loaded, just give the mesh the details
        if (a_tex.path.data() == embedded_filename) {
          out_texInfo.insert(out_texInfo.end(), { a_tex.accessId, a_tex.type });
          a_tex.ref_count++;
          texture_has_loaded = true;
          break;
        }
      }
      if (!texture_has_loaded) {
        // not already loaded, ok lets load it
        TextureInfo a_new_texture_info;
        //bool compressed = (ai_embedded_texture->mHeight == 0) ? true : false;
        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(1);
        int texture_size = ai_embedded_texture->mWidth * std::max(ai_embedded_texture->mHeight, 1u);
        unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(ai_embedded_texture->pcData), texture_size, &width, &height, &nrComponents, rgb_type);
        if (data) {
          switch (Settings::Get()->GetOptions().renderer) {
          case RenderingFramework::OPENGL:
            a_new_texture_info.accessId = OGLGraphics::Upload2DTex(data, width, height);
            if (a_new_texture_info.accessId != 0) {
              // add the new one to our list of loaded textures
              a_new_texture_info.path = embedded_filename;
              a_new_texture_info.type = typeName;
              // update our list of loaded textures
              mLoadedTextures.push_front(a_new_texture_info);
              // to return for draw info on this current mesh
              out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
              //std::cout << "loaded a new embedded texture @ " << a_new_texture_info.path << "!\n";
            }
            break;
          }
        }
      }
    }
    // try from file
    else {
      //regular file, check if it exists and read it
      // the 3 paths to try
      std::vector<std::string> potential_paths;
      // 1. the direct string (will probably fail)
      potential_paths.emplace_back(aiTmpStr.C_Str());
      // 2. the path based on where the model was loaded from (might work)
      std::string literal_path = ModelLoader::mModelDir + aiTmpStr.C_Str();
      potential_paths.emplace_back(literal_path);
      // 3. the last part of the given path (after '/' or '\\') appended to the path based on were the model was loaded from
      std::string from_model_path = ModelLoader::mModelDir + literal_path.substr(literal_path.find_last_of("/\\") + 1);  // all the way to the end
      potential_paths.emplace_back(from_model_path);
      // routine to see if we already have this texture loaded
      for (auto& a_tex : mLoadedTextures) {
        for (const auto& a_path : potential_paths) {
          if (a_tex.path.data() == a_path) {
            // texture already loaded, just give the mesh the details
            out_texInfo.insert(out_texInfo.end(), { a_tex.accessId, a_tex.type });
            a_tex.ref_count++;
            return 0;  // success
          }
        }
      }
      // wasn't already loaded, lets try to load it
      TextureInfo a_new_texture_info;
      int width = 0, height = 0, nrComponents = 0;
      stbi_set_flip_vertically_on_load(1);
      // try
      unsigned char* data = nullptr;
      for (const auto& a_path : potential_paths) {
        data = stbi_load(a_path.c_str(), &width, &height, &nrComponents, rgb_type);
        if (data) {
          switch (Settings::Get()->GetOptions().renderer) {
          case RenderingFramework::OPENGL:
            a_new_texture_info.accessId = OGLGraphics::Upload2DTex(data, width, height);
            break;
          }
          if (a_new_texture_info.accessId != 0) {
            // add the new one to our list of loaded textures
            a_new_texture_info.path = a_path;
            a_new_texture_info.type = typeName;
            mLoadedTextures.push_front(a_new_texture_info);
            // to return for draw info on this current mesh
            out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
            //std::cout << "loaded a new texture @ " << a_new_texture_info.path << "!\n";
            break;
          }
        }
      }
      stbi_image_free(data);
    }
  }

  // went through the above loop without error, mLoadedTextures & out_texInfo should be updated
  return 0;
}

}
}
