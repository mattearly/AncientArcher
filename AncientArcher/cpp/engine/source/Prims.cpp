#include <Prims.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <TextureLoader.h>
#include <iostream>

void Prims::render()
{
  if (_shader == nullptr)
  {
    throw "Shader not set for primative drawing.";
  }
  else
  {
    _shader->use();
    if (!_camera)
    {
      throw "Camera not set for primative drawing.";
    }
    else
    {
      _camera->updateViewMatrix(_shader.get());
    }
  }

  glEnable(GL_DEPTH_TEST);

  for (auto p : _objects)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, p.texID);

    glm::mat4 model = glm::mat4(1.0f);
    // step1: translate
    model = glm::translate(model, glm::vec3(p.position));
    // step2: rotations  -- not supported by colliders yet
    //  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // step3: scale
    model = glm::scale(model, glm::vec3(p.scale));

    _shader->setMat4("model", model);

    switch (p.type)
    {
    case PRIMTYPE::CUBE:
      Cube::instance()->drawCube();
      break;
    case PRIMTYPE::PLANE:
      Plane::instance()->drawPlane();
      break;
    case PRIMTYPE::SPHERE:
      Sphere::instance()->drawSphere();
      break;
    default: break;
    }
  }
}

void Prims::newObject(PRIMTYPE type, glm::vec3 pos, glm::vec3 scale, std::string texPath, bool isVisible)
{
  static TextureLoader tloader;
  primObject tmpobj;

  tmpobj.type = type;
  tmpobj.position = pos;
  tmpobj.scale = scale;
  tmpobj.visibility = isVisible;

  if (_texturePaths.empty())
  {
    // set object texture
    tmpobj.texID = tloader.load2DTexture(texPath);

    // save note of texture path loaded
    texture tex;
    tex.id = tmpobj.texID;
    tex.path = texPath;
    _texturePaths.push_back(tex);
  }
  else
  {
    bool texFound = false;
    for (auto f : _texturePaths)
    {
      if (f.path.compare(texPath) == 0)
      {
        tmpobj.texID = f.id;
        texFound = true;
        break;
      }
    }

    if (!texFound)
    {
      // set object texture
      tmpobj.texID = tloader.load2DTexture(texPath);

      // save note of texture path loaded
      texture tex;
      tex.id = tmpobj.texID;
      tex.path = texPath;
      _texturePaths.push_back(tex);
    }
  }

  _objects.push_back(tmpobj);
}

std::size_t Prims::count()
{
  return _objects.size();
}

void Prims::setShader(std::shared_ptr<Shader>& shader)
{
  _shader = shader;
}

void Prims::setCamera(std::shared_ptr<Camera>& camera)
{
  _camera = camera;
}