#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <assimp/scene.h>

struct Mesh
{
  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
  Mesh(unsigned int a, unsigned int b, unsigned int e);
};

struct AAModel
{
  std::vector<Mesh> meshVec;
};

class AAOGLGraphics
{
public:

  void drawModel(const AAModel* modelToDraw);

  void loadModel(std::string path, AAModel& modelToFill);

  void unloadModel(std::string path);

private:

  std::vector<std::string> mPathsLoaded;

  bool isLoaded(std::string path);

  AAModel loadElements(const aiScene* scene);

  int getAlreadyLoadedModelID(std::string path);

  Mesh loadMesh(const aiMesh* mesh);

  void pushToGraphicsCard(AAModel& aamodel);

  void unloadElements(std::string path);

  void unloadElements(int id);

};

