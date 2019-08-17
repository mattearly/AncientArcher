// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
// slightly modified by MJE
#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <vertex.h>
#include <texture.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Mesh {
public:
  /*  Mesh Data  */
  std::vector<vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<texture> textures;
  unsigned int VAO;

  /*  Functions  */
  // constructor
  Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures);

  // render the mesh
  void render(Shader* shader);

private:
  /*  Render data  */
  unsigned int VBO, EBO;

  /*  Functions    */
  // initializes all the buffer objects/arrays
  void setupMesh();
};
