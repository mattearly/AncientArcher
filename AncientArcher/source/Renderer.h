#pragma once

class Player;
class PrimativeManager;
class Entity;
class SkyboxRenderer;

class Renderer {
public:
  Renderer();
  ~Renderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(Player *player, PrimativeManager *primativeManager, SkyboxRenderer* skyboxRenderer, float deltaTime);

private:

  float width;

  void drawEntity(Entity *e, PrimativeManager *pm);

};

