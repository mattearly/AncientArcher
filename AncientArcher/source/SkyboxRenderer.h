#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
class Skybox;
class SkyboxRenderer
{
public:
  SkyboxRenderer(glm::mat4 projectionMatrix);
private:
  Skybox skybox;
  const float SIZE = 500f;
  const float VERTICES[] = {
      -SIZE,  SIZE, -SIZE,
      -SIZE, -SIZE, -SIZE,
       SIZE, -SIZE, -SIZE,
       SIZE, -SIZE, -SIZE,
       SIZE,  SIZE, -SIZE,
      -SIZE,  SIZE, -SIZE,

      -SIZE, -SIZE,  SIZE,
      -SIZE, -SIZE, -SIZE,
      -SIZE,  SIZE, -SIZE,
      -SIZE,  SIZE, -SIZE,
      -SIZE,  SIZE,  SIZE,
      -SIZE, -SIZE,  SIZE,

       SIZE, -SIZE, -SIZE,
       SIZE, -SIZE,  SIZE,
       SIZE,  SIZE,  SIZE,
       SIZE,  SIZE,  SIZE,
       SIZE,  SIZE, -SIZE,
       SIZE, -SIZE, -SIZE,

      -SIZE, -SIZE,  SIZE,
      -SIZE,  SIZE,  SIZE,
       SIZE,  SIZE,  SIZE,
       SIZE,  SIZE,  SIZE,
       SIZE, -SIZE,  SIZE,
      -SIZE, -SIZE,  SIZE,

      -SIZE,  SIZE, -SIZE,
       SIZE,  SIZE, -SIZE,
       SIZE,  SIZE,  SIZE,
       SIZE,  SIZE,  SIZE,
      -SIZE,  SIZE,  SIZE,
      -SIZE,  SIZE, -SIZE,

      -SIZE, -SIZE, -SIZE,
      -SIZE, -SIZE,  SIZE,
       SIZE, -SIZE, -SIZE,
       SIZE, -SIZE, -SIZE,
      -SIZE, -SIZE,  SIZE,
       SIZE, -SIZE,  SIZE
  };

  std::vector<std::string> texture_files = { "skybox_heresy_right", "skybox_heresy_left", "skybox_heresy_top", 
                                             "skybox_heresy_bottom", "skybox_heresy_back", "skybox_heresy_front" };

  int cube;
  int texture;
  Skybox skybox;
};

