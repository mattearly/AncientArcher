#include <World.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <vector>

/**
 * Loads up a primative renderer with the default settings and shader.
 */
World::World()
{

  // world position 0,0,0
  // yaw -90.f : forward facing down the -z axis  (should be 0.f is down the -z axis, why the f is it -90? #confusedprogramming)
  // pitch 0.0f : forward
  // field of view 80.f : pretty wide, slight fisheye
  _defaultWorldCamera = std::make_shared<Camera>(glm::vec3(0.f, 5.0f, 0.f), -90.f, -45.0f, 80.f);
  _defaultWorldShader = std::make_shared<Shader>("../AncientArcher/resource/world/primative.vert", "../AncientArcher/resource/world/primative.frag");

  _defaultWorldShader->use();
  glm::mat4 proj = _defaultWorldCamera->getProjectionMatrix();
  _defaultWorldShader->setMat4("projection", proj);


  _defaultWorldLighting = std::make_shared<Lighting>();

  //_defaultWorldLighting->updateConstantLightAmbient(glm::vec3(.09, 0.07, 0.07));
  //_defaultWorldLighting->updateConstantLightDirection(glm::vec3(0, -1, 0));
  //_defaultWorldLighting->updateConstantLightDiffuse(glm::vec3(.80, .70, .74));
  //_defaultWorldLighting->updateConstantLightSpecular(glm::vec3(.5, .5, .5));

  _defaultWorldLighting->setConstantLight(getShader());

  // debug
  std::cout << "number of shared Camera in world init " << _defaultWorldCamera.use_count() << std::endl;
  std::cout << "number of shared Shader in world init " << _defaultWorldShader.use_count() << std::endl;
  std::cout << "number of shared Lighting in world init " << _defaultWorldLighting.use_count() << std::endl;
  // -- ok

}

void World::update(float deltaTime)
{
  elapsedTime += deltaTime;

  if (elapsedTime > 2.0f) {
    timeTrigger = !timeTrigger;
    elapsedTime = 0.0f;
  }

  for (auto& e : *(getMovingEntites())) {
    e.moveBy(glm::vec3(0.f,
      0.f,
      (sin(elapsedTime * 3.14159 / 180) * (timeTrigger ? 1.0 : -1.0)))
    );
  }
}

/**
 * Renders all the objects on the std::vector<Entity> entities array.
 */
void World::render()
{
  _defaultWorldShader->use();

  _defaultWorldCamera->update(_defaultWorldShader.get());
  glEnable(GL_DEPTH_TEST);

  for (auto e : _stationaryEntities)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, e.gameItem.textureID);

    glm::mat4 model = glm::mat4(1.0f);
    // step1: translate
    model = glm::translate(model, glm::vec3(e.gameItem.loc));
    // step2: rotations  -- not supported by colliders yet
    //  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // step3: scale
    model = glm::scale(model, glm::vec3(e.gameItem.scale));

    _defaultWorldShader->setMat4("model", model);

    switch (e.gameItem.type) {
    case ENTITYTYPE::CUBE:
      drawCube();
      break;
    case ENTITYTYPE::PLANE:
      drawPlane();
      break;
    case ENTITYTYPE::SPHERE:
      drawSphere();
      break;
    default: break;
    }
  }
  for (auto e : _movingEntities)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, e.gameItem.textureID);

    glm::mat4 model = glm::mat4(1.0f);
    // step1: translate
    model = glm::translate(model, glm::vec3(e.gameItem.loc));
    // step2: rotations  -- not supported by colliders yet
    //  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // step3: scale
    model = glm::scale(model, glm::vec3(e.gameItem.scale));

    _defaultWorldShader.get()->setMat4("model", model);

    switch (e.gameItem.type) {
    case ENTITYTYPE::CUBE:
      drawCube();
      break;
    case ENTITYTYPE::PLANE:
      drawPlane();
      break;
    case ENTITYTYPE::SPHERE:
      drawSphere();
      break;
    default: break;
    }
  }
}

/**
 * Adds a built entity to the std::vector<Entity> entities array.
 */
void World::addToStationaryEntities(Entity entity)
{
  _stationaryEntities.push_back(entity);
}

void World::addToMovingEntities(Entity entity)
{
  _movingEntities.push_back(entity);
}

std::vector<Entity>* World::getEntites()
{
  return &_stationaryEntities;
}

Entity* World::getFirstEntity()
{
  return &_stationaryEntities[0];
  //return entities.data();
}

Entity* World::getFirstPlayerEntity()
{
  return &_players[0];
  //return players.data();
}

std::vector<Entity>* World::getMovingEntites()
{
  return &_movingEntities;
}

Entity* World::getFirstMovingEntity()
{
  return &_movingEntities[0];
}

Shader* World::getShader()
{
  return _defaultWorldShader.get();
}

std::shared_ptr<Shader>& World::getSharedShader()
{
  return _defaultWorldShader;
}

Lighting* World::getLight()
{
  return _defaultWorldLighting.get();
}

Camera* World::getCamera()
{
  return _defaultWorldCamera.get();
}

std::shared_ptr<Camera>& World::getSharedCamera()
{
  return _defaultWorldCamera;

}
std::size_t World::numberOfEntities()
{
  return _stationaryEntities.size();
}

void World::entityPopBack()
{
  if (_stationaryEntities.size() > 0)
  {
    _stationaryEntities.pop_back();
  }
}

// --- PRIVATE FUNCTIONS --- //

/**
 * Draws a cube. Loads up the cube primative if it isn't already loaded. Instantiate Entities and call render() to draw them.
 */
void World::drawCube()
{
  if (!cubeLoaded) {
    loadCube();
  }
  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

/**
 * Draws a plane. Loads up the plane primative if it isn't already loaded. Instantiate Entities and call render() to draw them.
 */
void World::drawPlane()
{
  if (!planeLoaded) {
    loadPlane();
  }
  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

/**
 * Draws a sphere. Loads up the sphere primative if it isn't already loaded. Instantiate Entities and call render() to draw them.
 */
void World::drawSphere()
{
  if (!sphereLoaded) {
    loadSphere();
  }
  glBindVertexArray(sphereVAO);

  //glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);

  glDrawElements(GL_TRIANGLES, sphereIndexSize, GL_UNSIGNED_INT, (void*)0);

  glBindVertexArray(0);

}

void World::loadCube() {
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

  std::cout << "cube Loaded to Graphics Card \n";

}

void World::loadPlane() {
  // plane with texture coords and normals
  float vertices[] = {
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

/**
 * Load a 36 sector, 18 stack sphere.
 * * sets sphereVAO and sphere VBO's
 * largely built from this source: http://www.songho.ca/opengl/gl_sphere.html
 */
void World::loadSphere()
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

  std::cout << "sphere Loaded to Graphics Card \n";

}