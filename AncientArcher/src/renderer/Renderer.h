#pragma once

class Pickups;
class PrimativeManager;

class Renderer {
public:
  Renderer();
  ~Renderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(Pickups *pickups, PrimativeManager *primativeManager, float deltaTime);

private:

  float width;

};

