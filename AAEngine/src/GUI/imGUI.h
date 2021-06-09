#pragma once
#include <imgui.h>
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
namespace AA{
class imGUI {
public:
  void Init(GLFWwindow* window, char* gl_num_shading_lang_versions);
  void Shutdown();
  void NewFrame();
  void Update();
  void Render();
private:
};

} // end namespace AA
