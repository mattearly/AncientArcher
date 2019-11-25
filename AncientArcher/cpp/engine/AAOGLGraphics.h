#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include "AAGameObject.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
};

class AAOGLGraphics
{
public:
  static AAOGLGraphics* getInstance();

  AAGameObject loadModelWithAssimpToOpenGL(std::string path);

  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);

private:

};

