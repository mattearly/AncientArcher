/*
  Intended usage: scene to load and render
*/
#pragma once
#include <Model.h>
#include <Prims.h>
#include <Skybox.h>
#include <vector>
#include <string>
class Scene
{
public:
  void render();

  void setupSkybox(std::shared_ptr<Camera>& cam, std::vector<std::string> cubeMapFiles);

  void addModel(Model model);
  void addPrimGroup(Prims prims);

  void setShader(std::shared_ptr<Shader>& shader);
  void setCamera(std::shared_ptr<Camera>& camera);
  
private:
  std::vector<Model> _models;
  std::vector<Prims> _primGroups;
  Skybox* sky;

  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Camera> _camera;

};