#pragma once
class Plane
{
public:

  static Plane* instance();

  void drawPlane();

private:

  void loadPlane();
  unsigned int planeVAO = 0;
  unsigned int planeVBO = 0;

  Plane();
  Plane(Plane const&) = delete;
  Plane& operator=(Plane const&) = delete;

  static Plane* _plane;
};