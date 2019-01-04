#pragma once
#include <string>
class DiffuseTexture {
public:
  DiffuseTexture();
  ~DiffuseTexture();

  void loadDiffuseTexture(std::string path);

  void activate(int n);

private:

  static constexpr int MAXDIFFUSETEXTURES = 16;  // 0 - 15

  int active_diffuse;

  int num_loaded_diffuse;

  unsigned int texture[MAXDIFFUSETEXTURES]; // 16 array

  int width, height, nrChannel;

  bool initiated;
};

