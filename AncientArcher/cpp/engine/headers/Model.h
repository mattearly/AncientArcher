// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
// slightly modified MJE
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Mesh.h>
#include <Shader.h>
#include <string>
#include <vector>

unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
  /*  Model Data */
  std::vector<texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  std::vector<Mesh> meshes;
  std::string directory;
  bool gammaCorrection;

  /*  Functions   */
  // constructor, expects a filepath to a 3D model.
  Model(const std::string& path, bool gamma = false);

  // draws the model, and thus all its meshes
  void render(Shader* shader);

private:
  /*  Functions   */
  // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
  void loadModel(const std::string& path);

  // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
  void processNode(aiNode* node, const aiScene* scene);

  Mesh processMesh(aiMesh* mesh, const aiScene* scene);

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
