#include "TextureLoader.h"
#include "OpenGL/OGLGraphics.h"
#include "ModelLoader.h"
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif

namespace AA {

// internal saved list of loaded textures to manage
static std::forward_list<TextureInfo> AllLoadedTextures;

std::unordered_map<unsigned int, std::string> TextureLoader::LoadAllTextures(const aiScene* scene, const aiMaterial* ai_material, const std::string& orig_filepath) {

  std::unordered_map<unsigned int, std::string> all_loaded_textures;

//#ifdef _DEBUG
//   std::cout << "Texture Load Debug::";
//  int shading_model;
//  ai_material->Get(AI_MATKEY_SHADING_MODEL, shading_model);
//  switch (shading_model) {
//  case aiShadingMode_Flat:
//    std::cout << "Shading mode is aiShadingMode_Flat\n";
//    break;
//  case aiShadingMode_Gouraud:
//    std::cout << "Shading mode is aiShadingMode_Gouraud\n";
//    break;
//  case aiShadingMode_Phong:
//    std::cout << "Shading mode is aiShadingMode_Phong\n";
//    break;
//  case aiShadingMode_Blinn:
//    std::cout << "Shading mode is aiShadingMode_Blinn\n";
//    break;
//  case aiShadingMode_Toon:
//    std::cout << "Shading mode is aiShadingMode_Toon\n";
//    break;
//  case aiShadingMode_OrenNayar:
//    std::cout << "Shading mode is aiShadingMode_OrenNayar\n";
//    break;
//  case aiShadingMode_Minnaert:
//    std::cout << "Shading mode is aiShadingMode_Minnaert\n";
//    break;
//  case aiShadingMode_NoShading:
//    std::cout << "Shading mode is aiShadingMode_NoShading\n";
//    break;
//  case aiShadingMode_Fresnel:
//    std::cout << "Shading mode is aiShadingMode_Fresnel\n";
//    break;
//  default: break;
//  }
//#endif

  // get the albedo (diffuse) textures
  std::unordered_map<unsigned int, std::string> albedo_textures;
  if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_DIFFUSE, "Albedo", orig_filepath, albedo_textures) == 0) {
    for (auto& a_tex : albedo_textures) {
      all_loaded_textures.insert(all_loaded_textures.end(), a_tex);
    }
  }

  std::unordered_map<unsigned int, std::string> specular_textures;
  if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_SPECULAR, "Specular", orig_filepath, specular_textures) == 0) {
    for (auto& s_tex : specular_textures) {
      all_loaded_textures.insert(all_loaded_textures.end(), s_tex);
    }
  }

  std::unordered_map<unsigned int, std::string> normal_textures;
  if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_NORMALS, "Normal", orig_filepath, normal_textures) == 0) {
    for (auto& n_tex : normal_textures) {
      all_loaded_textures.insert(all_loaded_textures.end(), n_tex);
    }
  }

  // if finding normal textures failed, see if it is actually called a heightmap (.obj files soemtimes have this according to https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
  if (normal_textures.empty()) {
    if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_HEIGHT, "Normal", orig_filepath, normal_textures) == 0) {
      for (auto& n_tex : normal_textures) {
        all_loaded_textures.insert(all_loaded_textures.end(), n_tex);
      }
    }
  }

  // emissive textures will glow on the lit shader
  std::unordered_map<unsigned int, std::string> emissive_textures;
  if (TextureLoader::loadMaterialTextures(scene, ai_material, aiTextureType_EMISSIVE, "Emission", orig_filepath, emissive_textures) == 0) {
    for (auto& e_tex : emissive_textures) {
      all_loaded_textures.insert(all_loaded_textures.end(), e_tex);
    }
  }

  //  float specStrength = 0.f; // default value, remains unmodified if we fail.
  //  if (aiGetMaterialFloat(ai_material, AI_MATKEY_SHININESS_STRENGTH, (float*)&specStrength) == AI_FAILURE) 
  //  {
  //#ifdef _DEBUG
  //    std::cout << "specStrength not found\n";
  //#endif
  //  }

  //  aiColor4D emissive_color;
  //  if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_EMISSIVE, (aiColor4D*)&emissive_color) == AI_FAILURE) {
  //#ifdef _DEBUG
  //    std::cout << "emissive_color not found\n";
  //#endif
  //  }

  return all_loaded_textures;
}

void TextureLoader::increment_given_texture_ids(const std::unordered_map<uint32_t, std::string>& list) {
  for (const auto& l : list) {
    for (auto& t : AllLoadedTextures) {
      if (l.first == t.accessId) {
        t.ref_count++;
      }
    }
  }
}

unsigned int TextureLoader::LoadTexture(const std::string& texture_path) {
  for (auto& a_tex : AllLoadedTextures) {
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
  // for unflipped opengl coords, flip vertically on load to true
  stbi_set_flip_vertically_on_load(0);
  texture_data = stbi_load(texture_path.c_str(), &width, &height, &nrComponents, rgb_type);
  if (texture_data) {
    a_new_texture_info.accessId = OGLGraphics::Upload2DTex(texture_data, width, height);
    if (a_new_texture_info.accessId != 0) {
      // add the new one to our list of loaded textures
      a_new_texture_info.path = texture_path;
      a_new_texture_info.type = "Albedo";  // todo: make this sync better
      a_new_texture_info.ref_count = 1;
      AllLoadedTextures.push_front(a_new_texture_info);
#ifdef _DEBUG
      std::cout << "loaded a new texture @ " << texture_path << "!\n";
#endif
    }
    stbi_image_free(texture_data);
  }
  return a_new_texture_info.accessId;
}

unsigned int TextureLoader::LoadCubeMapTexture(const std::vector<std::string>& six_texture_paths) {
  if (six_texture_paths.size() != 6)
    throw("wrong number of textures");
  int width = 0, height = 0, nrChannel = 0;
  stbi_set_flip_vertically_on_load(0); // tell stb_image.h to not flip loaded texture's on the y-axis.
  std::vector<unsigned char*> data;
  data.resize(6);
  for (auto i = 0; i < 6; ++i) {
    data[i] = stbi_load(six_texture_paths[i].c_str(), &width, &height, &nrChannel, STBI_rgb_alpha);
  }
  unsigned int return_id = OGLGraphics::UploadCubeMapTex(data, width, height, nrChannel == 4);
#ifdef _DEBUG
  printf("LoadedCubeMap(id:%i)\n", return_id);
#endif
  return return_id;
}

void TextureLoader::UnloadTexture(const std::unordered_map<unsigned int, std::string>& texture_draw_ids) {
  for (const auto& texIt : texture_draw_ids) {
    for (auto loaded_tex = AllLoadedTextures.begin(); loaded_tex != AllLoadedTextures.end(); loaded_tex++) {
      if (texIt.first == loaded_tex->accessId) {
        loaded_tex->ref_count--;
        if (loaded_tex->ref_count == 0) {
          OGLGraphics::DeleteTex(loaded_tex->accessId);
        }
      }
    }

    // sync local textures list
    AllLoadedTextures.remove_if([](const TextureInfo& ti) -> bool { return (ti.ref_count == 0) ? true : false; });
  }


}

int TextureLoader::loadMaterialTextures(const aiScene* scn, const aiMaterial* mat, aiTextureType type, std::string typeName, std::string orginalFilePath, std::unordered_map<unsigned int, std::string>& out_texInfo) {
  unsigned int num_textures = mat->GetTextureCount(type);
  if (num_textures == 0) {
    return -3;
  }

  auto rgb_type = STBI_rgb_alpha;

  for (unsigned int i = 0; i < num_textures; ++i) {
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

    // pathing options we use later to help finding textures based on model path
    std::size_t the_last_slash = orginalFilePath.find_last_of("/\\") + 1;
    std::size_t the_last_dot = orginalFilePath.find_last_of(".");
    std::string model_dir = orginalFilePath.substr(0, the_last_slash);  // path to filename's dir
    std::string model_file_extension = orginalFilePath.substr(
      static_cast<std::basic_string<char,
      std::char_traits<char>,
      std::allocator<char>>::size_type>(the_last_dot) + 1);  // get the file extension (type of file)
    std::string model_file_name = orginalFilePath.substr(
      the_last_slash,
      static_cast<std::basic_string<char,
      std::char_traits<char>,
      std::allocator<char>>::size_type>(the_last_dot) - the_last_slash);  // get the name of the file

    // starting attempts at loading textures from a variety of possibilities
    // try from embedded
    const aiTexture* ai_embedded_texture = scn->GetEmbeddedTexture(aiTmpStr.C_Str());


    // trying from embedded textures first
    // when this happens we don't need to load it from a texture path, but from the memory
    if (ai_embedded_texture) {
      bool texture_has_loaded = false;
      std::string embedded_filename = ai_embedded_texture->mFilename.C_Str();
      if (embedded_filename == "") {
        // try fix for glb embedded textures and not having to reload them
        embedded_filename = model_file_name + "." + model_file_extension + "/" + typeName;
      }

      // see if it has already been loaded previously to reuse
      for (auto& a_tex : AllLoadedTextures) {
        // if texture path already loaded, just give the mesh the details
        if (a_tex.path.data() == embedded_filename) {
          out_texInfo.insert(out_texInfo.end(), { a_tex.accessId, a_tex.type });
#ifdef _DEBUG
          std::cout << "TEXTURE REUSED embedded (" << a_tex.type << "::" << a_tex.path << ")\n";
#endif
          a_tex.ref_count++;
          texture_has_loaded = true;
          break;
        }
      }

      // not already loaded, lets try from embedded, this should succeed if it gets here
      if (!texture_has_loaded) {
        TextureInfo a_new_texture_info;
        //bool compressed = (ai_embedded_texture->mHeight == 0) ? true : false;
        int width, height, nrComponents;
        // for unflipped opengl coords, flip vertically on load to true
        stbi_set_flip_vertically_on_load(0);
        int texture_size = ai_embedded_texture->mWidth * std::max(ai_embedded_texture->mHeight, 1u);
        unsigned char* data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(ai_embedded_texture->pcData), texture_size, &width, &height, &nrComponents, rgb_type);
        if (data) {
          a_new_texture_info.accessId = OGLGraphics::Upload2DTex(data, width, height);
          if (a_new_texture_info.accessId != 0) {
            // add the new one to our list of loaded textures
            a_new_texture_info.path = embedded_filename;
            a_new_texture_info.type = typeName;
            // update our list of loaded textures
            AllLoadedTextures.push_front(a_new_texture_info);
            // to return for draw info on this current mesh
            out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
#ifdef _DEBUG
            std::cout << "TEXTURE ADDED\n  - embedded path: " << a_new_texture_info.path << "\n  - type: " << a_new_texture_info.type << "\n";
#endif
          }
        }
      }
    }
    // ELSE: textures are not embedded
    // try from 3 most likely paths as detailed below
    else {
      //regular file, check if it exists and read it
      // the 3 paths to try
      std::vector<std::string> potential_paths;
      // 1. the direct string (will probably fail)
      potential_paths.emplace_back(aiTmpStr.C_Str());
      // 2. the path based on where the model was loaded from (might work)
      std::string literal_path = model_dir + aiTmpStr.C_Str();
      potential_paths.emplace_back(literal_path);
      // 3. the last part of the given path (after '/' or '\\') appended to the path based on were the model was loaded from
      std::string from_model_path = model_dir + literal_path.substr(literal_path.find_last_of("/\\") + 1);  // all the way to the end
      potential_paths.emplace_back(from_model_path);
      // routine to see if we already have this texture loaded
      for (auto& a_tex : AllLoadedTextures) {
        for (const auto& a_path : potential_paths) {
          if (a_tex.path.data() == a_path) {
            // texture already loaded, just give the mesh the details
            out_texInfo.insert(out_texInfo.end(), { a_tex.accessId, a_tex.type });
#ifdef _DEBUG
            std::cout << "TEXTURE REUSED (" << a_tex.type << "::" << a_tex.path << ")\n";
#endif
            a_tex.ref_count++;
            return 0;  // success
          }
        }
      }
      // wasn't already loaded, lets try to load it
      TextureInfo a_new_texture_info;
      int width = 0, height = 0, nrComponents = 0;
      // for unflipped opengl coords, flip vertically on load to true
      stbi_set_flip_vertically_on_load(0);
      // try
      unsigned char* data = nullptr;
      for (const auto& a_path : potential_paths) {
        data = stbi_load(a_path.c_str(), &width, &height, &nrComponents, rgb_type);
        if (data) {
          // we have data that goes to the graphics card
          a_new_texture_info.accessId = OGLGraphics::Upload2DTex(data, width, height);
          if (a_new_texture_info.accessId != 0) {
            // add the new one to our list of loaded textures for management
            a_new_texture_info.path = a_path;
            a_new_texture_info.type = typeName;
            AllLoadedTextures.push_front(a_new_texture_info);
            // to return for draw info on this current mesh
            out_texInfo.insert(out_texInfo.end(), { a_new_texture_info.accessId, a_new_texture_info.type });
#ifdef _DEBUG
            std::cout << "TEXTURE ADDED\n  - path: " << a_new_texture_info.path << "\n  - type: " << a_new_texture_info.type << "\n";
#endif
            break;  // break out of for loop
          }
        }
      }
      stbi_image_free(data);
    }
  }

  // went through the above loop without error, AllLoadedTextures & out_texInfo should be updated
  return 0;
}

} // end namespace AA
