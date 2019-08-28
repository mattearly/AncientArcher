#pragma once
class Sphere
{
public:
  
  static Sphere* instance();

  void drawSphere();

private:
 
  void loadSphere();
  
  unsigned int sphereVAO = 0;
  unsigned int sphereVBO = 0;
  unsigned int sphereIBO = 0;
  unsigned int sphereIndexSize = 0;

  Sphere();
  Sphere(Sphere const&) = delete;  // ? or just private and not deleted
  Sphere& operator=(Sphere const&) = delete;  // ? or just private and not deleted

  static Sphere* _sphere;
};