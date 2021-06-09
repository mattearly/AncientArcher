#include "imGUI.h"
#include "../Settings/Settings.h"
namespace AA {

void imGUI::Init(GLFWwindow* window, char* gl_num_shading_lang_versions) {
  switch (Settings::Get()->GetOptions().renderer) {
  case RenderingFramework::OPENGL:
#if _DEBUG
    IMGUI_CHECKVERSION();
#endif
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(gl_num_shading_lang_versions);
    break;
  default:
    throw("unsupported");
  }
}

void imGUI::Shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void imGUI::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void imGUI::Update() {
  ImGui::Text("Hello, world!");
}

void imGUI::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


}
