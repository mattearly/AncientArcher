#include <Primitive.h>
#include <glad/glad.h>
#include <vector>
Primitive::Primitive()
{
  loadCube();
  loadPlane();
  loadSphere();
}

void Primitive::drawCube()
{
  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void Primitive::drawPlane()
{
  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Primitive::drawSphere()
{
  glBindVertexArray(sphereVAO);

  //glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);

  glDrawElements(GL_TRIANGLES, sphereIndexSize, GL_UNSIGNED_INT, (void*)0);

  glBindVertexArray(0);
}

void Primitive::loadCube()
{
  // cube with texture coords and normals
  const float vertices[] = {
    // positions            // normals             // texture coords
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,     0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,     1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,     0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,     0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
  };

  /* set up an area to store vertex data */
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  // normal attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

  // texture coord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


  // unbind buffers
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  cubeLoaded = true;

  //std::cout << "cube Loaded to Graphics Card \n";
}

void Primitive::loadPlane()
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

  planeLoaded = true;
}

void Primitive::loadSphere()
{
  // --- PREPARE SPHERE VERTICES --- //
  const float radius = 0.5f;
  const float PI = 3.141592f;
  const int sectorCount = 36;
  const int stackCount = 18;

  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texCoords;

  float x, y, z, xy;                              // vertex position
  float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
  float s, t;                                     // vertex texCoord

  float sectorStep = 2 * PI / sectorCount;
  float stackStep = PI / stackCount;
  float sectorAngle, stackAngle;

  for (int i = 0; i <= stackCount; ++i)
  {
    stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
    xy = radius * cosf(stackAngle);             // r * cos(u)
    z = radius * sinf(stackAngle);              // r * sin(u)

    // add (sectorCount+1) vertices per stack
    // the first and last vertices have same position and normal, but different tex coords
    for (int j = 0; j <= sectorCount; ++j)
    {
      sectorAngle = j * sectorStep;           // starting from 0 to 2pi

      // vertex position (x, y, z)
      x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
      y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      // normalized vertex normal (nx, ny, nz)
      nx = x * lengthInv;
      ny = y * lengthInv;
      nz = z * lengthInv;
      normals.push_back(nx);
      normals.push_back(ny);
      normals.push_back(nz);

      // vertex tex coord (s, t) range between [0, 1]
      s = (float)j / sectorCount;
      t = (float)i / stackCount;
      texCoords.push_back(s);
      texCoords.push_back(t);
    }
  }

  // --- CREATE INDICIES --- //
  // generate CCW index list of sphere triangles
  std::vector<int> indices;
  int k1, k2;
  for (int i = 0; i < stackCount; ++i)
  {
    k1 = i * (sectorCount + 1);     // beginning of current stack
    k2 = k1 + sectorCount + 1;      // beginning of next stack

    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
    {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0)
      {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stackCount - 1))
      {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }

  /*unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
  unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3; }
  unsigned int getTexCoordCount() const { return (unsigned int)texCoords.size() / 2; }
  unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
*/

// --- LOAD TO GRAPHICS CARD --- //
  glGenVertexArrays(1, &sphereVAO);
  glGenBuffers(1, &sphereVBO);

  glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
  glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glBindVertexArray(sphereVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

  // normal attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

  // texture coord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

  // unbind buffers
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glBindVertexArray(0);

  glGenBuffers(1, &sphereIBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  sphereIndexSize = (unsigned int)indices.size();

  sphereLoaded = true;

  //std::cout << "sphere Loaded to Graphics Card \n";
}
