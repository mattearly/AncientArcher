#pragma once
class Cube
{
public:

  static Cube* instance();

  void drawCube();

private:

  void loadCube();
  unsigned int cubeVAO = 0;
  unsigned int cubeVBO = 0;

  Cube();
  Cube(Cube const&) = delete;  // ? or just private and not deleted
  Cube& operator=(Cube const&) = delete;  // ? or just private and not deleted

  static Cube* _cube;
};