#include "Cube.h"
#include "OpenGL/OGLGraphics.h"
#ifdef _DEBUG
#include<iostream>
#endif
namespace AA {
static unsigned int vao_to_the_cube = 0;
static bool cube_loaded = false;

unsigned int load_cube() {
  if (cube_loaded) {
#ifdef _DEBUG
    std::cout << "cube already loaded \n";
#endif
    return vao_to_the_cube;
  }

#ifdef _DEBUG
  std::cout << "loading cube... \n";
#endif

  const float SIZE = 1.f;

  const float cubeVertices[] = {
    // positions
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

  vao_to_the_cube = OGLGraphics::UploadMesh(cubeVertices, sizeof(cubeVertices) / sizeof(cubeVertices[0]));
#ifdef _DEBUG
  std::cout << "cube loaded, id = " << vao_to_the_cube << '\n';
#endif

  cube_loaded = true;
  return vao_to_the_cube;

}

}  // end namespace AA