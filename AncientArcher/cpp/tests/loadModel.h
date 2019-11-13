#pragma once
#include <string>
#include "../engine/AAOGLGraphics.h"
#include <iostream>

extern void showModelHeaderInfo(std::string path)
{
  AAOGLGraphics test;

  int return_value = test.loadModel(path);

  std::cout << "loadModelTest Returned: " << return_value << '\n';

  test.outputModelData();
}

extern void showModelHeaderInfo(std::vector<std::string> paths)
{
  AAOGLGraphics test;

  for (auto i = 0; i < paths.size(); ++i)
  {
    int return_value = test.loadModel(paths[i]);
    std::cout << i << ". loadModel Returned: " << return_value << '\n';
  }

  test.outputModelData();
}