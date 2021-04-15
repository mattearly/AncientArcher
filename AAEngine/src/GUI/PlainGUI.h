#pragma once
#include "../../include/AncientArcher/Types.h"
#include "../Renderer/OpenGL/OGLShader.h"
#include <vector>
namespace AA {

struct Slider {
  vec2 bot_left_loc;
  f32 low_val;
  f32 high_val;
  f32 bar_thickness;
  f32 bar_length;
  bool horizontal;
};

class PlainGUI {
public:
  PlainGUI();
  ~PlainGUI();
  //void AddSlider(Slider slider, void(*resulthandler)(float result));

  void InitShader();
  void UpdateUniforms(vec2 resolution, vec2 mouse_loc, float time);
  OGLShader* Shader;
  void Draw();
  int vao_id;
  int ind_size;

private:
  std::vector<Slider> p_Sliders;

  f32 total_time = 0;
};

}