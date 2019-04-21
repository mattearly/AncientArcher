#pragma once

class Player;
class PrimativeManager;
class Entity;

class Renderer {
public:
  Renderer();
  ~Renderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(Player *player, PrimativeManager *primativeManager, float deltaTime);

private:

  float width;

  void drawEntity(Entity *e, PrimativeManager *pm);

};

