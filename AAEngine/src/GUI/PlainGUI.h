#pragma once
#include "../../include/AncientArcher/Types.h"
#include "../Renderer/OpenGL/OGLShader.h"
#include <vector>
namespace AA {

struct Button {
  Button(vec2 pos, float width, float height, float alpha);
  vec2 pos;
  float width;
  float height;
  mat4 transformation;
  float alpha;
};

struct Slider {
  vec2 bot_left_loc;
  f32 low_val;
  f32 high_val;
  f32 bar_thickness;
  f32 bar_length;
  bool horizontal;
  mat4 transformation;
};

class PlainGUI {
public:
  PlainGUI();
  ~PlainGUI();

  void AddButton(vec2 pos, float width, float height, float alpha);

  //void AddSlider(Slider slider, void(*resulthandler)(float result));

  void InitShader();

  void UpdateUniforms(vec2 resolution, vec2 mouse_loc, float elapsed_time);

  OGLShader* Shader;

  void Draw();



private:
  std::vector<Slider> p_Sliders;

  std::vector<Button> p_Buttons;

  int vao_id;

  int ind_size;
  
  f32 total_time = 0;
};

}