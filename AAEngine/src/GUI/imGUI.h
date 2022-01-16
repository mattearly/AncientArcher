#pragma once
#include <imgui.h>
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
namespace AA {
class imGUI {
public:
  imGUI();
  void InitOpenGL(GLFWwindow* window);
  void Shutdown();
  void NewFrame();
  //void Update();  // updates done elsewhere
  void Render();
private:
  bool isInit;
};

} // end namespace AA
