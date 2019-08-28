#include "..\headers\Scene.h"
#include <Scene.h>

//draw all visable objects
void Scene::render()
{
  // draw primatives
  for (auto pg : _primGroups)
  {
    pg.render();
  }

  // draw sky
  sky->render();

}

void Scene::setupSkybox(std::shared_ptr<Camera>& cam, std::vector<std::string> cubeMapFiles)
{
  if (sky == nullptr)
  {
    sky = new Skybox(cam, cubeMapFiles);
  }
  else
  {
    throw "Skybox already initialized.";
  }
}

void Scene::addModel(Model model)
{
  _models.push_back(model);
}

void Scene::addPrimGroup(Prims prims)
{
  if (!_shader)
  {
    throw("Scene shader is null.");
  }
  prims.setShader(_shader);
  if (!_camera)
  {
    throw("Scene camera is null.");
  }
  prims.setCamera(_camera);

  _primGroups.push_back(prims);
}

void Scene::setShader(std::shared_ptr<Shader>& shader)
{
  _shader = shader;
}

void Scene::setCamera(std::shared_ptr<Camera>& camera)
{
  _camera = camera;
}
