// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
// slightly modified by MJE
#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <Camera.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <vertex.h>
#include <texture.h>

struct BoneInfo
{
  std::string Name;
  glm::mat4 BoneOffset = glm::mat4(0);
  glm::mat4 FinalTransformation = glm::mat4(0);   // for live processing
};

class Mesh 
{
public:

  // initialize a mesh that doesn't have bones and/or animations
  Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures);

  // initialize a mesh that has bones and/or animations
  Mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures, std::vector<BoneInfo> bones);
  
  // update animated meshes
  //void update(float deltaTime);

  // render the mesh
  void render(Shader* shader, Camera* camera);
  // bone info for translating the bone rotations into proper space before render

  std::vector<BoneInfo> boneInfo;
protected:

private:

  // initializes all the buffer objects/arrays
  void setupMesh();

  // vertices is the core mesh data for shaders
  std::vector<vertex> _vertices;
  // for iterating over vertices
  std::vector<unsigned int> _indices;
  // texture information
  std::vector<texture> _textures;


  // Vertex Array Object
  unsigned int _VAO;
  // Vertex Buffer Object
  unsigned int _VBO;
  // Element Buffer Object
  unsigned int _EBO;

};
