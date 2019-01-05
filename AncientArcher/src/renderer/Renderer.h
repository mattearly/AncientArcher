#pragma once

class Pickups;
class PrimativeManager;
class Lighting;

class Renderer {
public:
  Renderer();
  ~Renderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(Pickups *pickups, PrimativeManager *primativeManager, Lighting *lighting, float deltaTime);

private:

  int width;

};

