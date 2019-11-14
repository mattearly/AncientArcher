#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <assimp/scene.h>

struct AAOGLModel
{
  int numVertices = 0,  numFaces = 0, numMeshes = 0;
  int numTextures = 0, numMaterials = 0, numAnimations = 0, numBones = 0;

  /* numBones = 0, numPositions = 0, numTexels = 0, numNormals = 0,*/

  struct Mesh {
    std::vector<glm::vec3> vertex;  //v   x,y,z  
    std::vector<glm::vec2> texel;   //vt  u,v
    std::vector<glm::vec3> normal;  //vn  x,y,z
    std::vector<glm::vec3> faces;   //f   (v,vt,vn v,vt,vn v,vt,vn)
  };

  std::vector<Mesh> meshes;

};

class AAOGLGraphics
{
public:

  void drawModel(int id);

  int loadModel(std::string path);

  void outputModelData();

private:

  std::vector<AAOGLModel> mModels;

  std::vector<std::string> mPathsLoaded;

  bool isLoaded(std::string path);

  AAOGLModel loadElements(const aiScene* scene);

  int getAlreadyLoadedModelID(std::string path);

};

