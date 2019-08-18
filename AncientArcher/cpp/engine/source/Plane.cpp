#include <Plane.h>
#include <glad/glad.h>
#include <iostream>

Plane* Plane::_plane = nullptr;

Plane* Plane::instance()
{
  if (!_plane)
  {
    _plane = new Plane();
  }

  return _plane;
}

Plane::Plane()
{
  loadPlane();
}

void Plane::drawPlane()
{
  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Plane::loadPlane()
{
  // plane with texture coords and normals
  float vertices[] =
  {
    // positions               // normals       //text cords    
    -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
     0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
     0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    -0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f
  };

  /* set up an area to store vertex data */
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);

  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(planeVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

  // normal attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

  // texture coord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  std::cout << "Plane Loaded to Graphics Card \n";
}
