#pragma once
#include "../../include/AncientArcher/Types.h"
#include "../Renderer/OpenGL/OGLShader.h"
#include <vector>
namespace AA {

struct Button {
  Button(vec2 pos, f32 width_scale, f32 height_scale, vec3 color, f32 alpha);
  vec2 pos;
  f32 width;
  f32 height;
  mat4 transformation;
  vec3 color;
  f32 alpha;
  u32 texture_id;
  tf overlapped = false;
};

//struct Slider {
//  vec2 bot_left_loc;
//  f32 low_val;
//  f32 high_val;
//  f32 bar_thickness;
//  f32 bar_length;
//  bool horizontal;
//  mat4 transformation;
//};

class PlainGUI {
public:
  PlainGUI();
  ~PlainGUI();

  void AddButton(vec2 pos, f32 width_scale, f32 height_scale, vec3 color, f32 alpha);

  //void AddSlider(Slider slider, void(*resulthandler)(float result));

  void ShowInterface();

  void HideInterface();

  void InitShader();

  void UpdateMouseLoc(vec2 mouse_loc);

  OGLShader* Shader;

  void Draw();

private:

  void UpdateMouseOverlap(vec2 mouse_loc);
  
  void UpdateUniforms(vec2 resolution, vec2 mouse_loc, f32 elapsed_time);

  //std::vector<Slider> p_Sliders;

  std::vector<Button> p_Buttons;

  i32 vao_id;

  i32 ind_size;
  
  f32 total_time = 0;

  bool p_HideAll = false;
};

}