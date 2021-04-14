#pragma once
#include "../Base/Types.h"
#include "../Renderer/OpenGL/OGLShader.h"
namespace AA{
struct Slider {
  vec2 bot_left_loc;
  float low_val; 
  float high_val;
  float bar_thickness;
  float bar_length;
  bool horizontal;
  };
class PlainGUI {
public:
  PlainGUI();
  void AddSlider(Slider slider, void(*resulthandler)(float result));
  void InitShader();
  static OGLShader* Shader;
  void Draw();
  int vao_id;
  int ind_size;
private:
  std::vector<Slider> p_Sliders;
};

}