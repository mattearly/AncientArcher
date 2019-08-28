/* 
  This class is intended to store the information to draw primatives.
*/
#pragma once
#include <Cube.h>
#include <Sphere.h>
#include <Plane.h>
#include <Shader.h>
#include <Camera.h>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <glm/glm.hpp>

enum PRIMTYPE { CUBE, SPHERE, PLANE };

class Prims
{
  struct primObject
  {
    PRIMTYPE type;
    glm::vec3 position;
    glm::vec3 scale;
    unsigned int texID;
    bool visibility;
  };

  struct texture
  {
    unsigned int id = 0;
    std::string path;
  };

public:
  void render();
  void newObject(PRIMTYPE type, glm::vec3 pos, glm::vec3 scale, std::string texPath, bool isVisible = true);

  void setShader(std::shared_ptr<Shader>& shader);
  void setCamera(std::shared_ptr<Camera>& camera);

  std::size_t count();

private:
  std::vector<primObject> _objects;

  std::vector<texture> _texturePaths;

  std::shared_ptr<Shader> _shader;

  std::shared_ptr<Camera> _camera;

};