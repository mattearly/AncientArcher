#include "PlainGUI.h"
#include <vector>
#include "../Renderer/OpenGL/OGLGraphics.h"
#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
namespace AA {
PlainGUI::PlainGUI() {
  InitShader();
}

PlainGUI::~PlainGUI() {
  delete Shader;
  Shader = NULL;
}

void PlainGUI::AddButton(vec2 pos, float width_scale, float height_scale, float alpha) {
  p_Buttons.emplace_back(pos, width_scale, height_scale, alpha);
}

//void PlainGUI::AddSlider(Slider slider, void(*resulthandler)(float result)) {
//  if (slider.bot_left_loc.x < -1.f || slider.bot_left_loc.y < -1.f)
//    throw("beyond -1");
//  if (slider.bot_left_loc.x > 1.f || slider.bot_left_loc.y > 1.f)
//    throw("beyond 1");
//
//  std::vector<vec3> points;
//  points.push_back(vec3(slider.bot_left_loc.x, slider.bot_left_loc.y, 0.f));  //bot left  ind:0
//  if (slider.horizontal) {
//    points.push_back(vec3(slider.bot_left_loc.x, slider.bot_left_loc.y + slider.bar_thickness, 0.f));  //top left  1
//    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_length, slider.bot_left_loc.y + slider.bar_thickness, 0.f)); //top right  2
//    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_length, slider.bot_left_loc.y, 0.f));  // bot right  3
//  } else  //vertical
//  {
//    points.push_back(vec3(slider.bot_left_loc.x, slider.bot_left_loc.y + slider.bar_length, 0.f));  //top left  1
//    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_thickness, slider.bot_left_loc.y + slider.bar_length, 0.f));  //top right  2
//    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_thickness, slider.bot_left_loc.y, 0.f));  // bot right  3
//  }
//
//  std::vector<u32> indices = { 0, 1, 2, 2, 3, 0 };
//
//  vao_id = OGLGraphics::UploadVerts(points, indices);
//  ind_size = sizeof(indices) / sizeof(indices[0]);  // should be 6
//}

void PlainGUI::InitShader() {
  if (Shader)
    return;

  std::vector<vec2> square_points = {
    vec2(-1, 1),
    vec2(-1, -1),
    vec2(1, 1),
    vec2(1, -1),
  };

  vao_id = OGLGraphics::Upload2DVerts(square_points);

  const char* vert =
    "#version 430 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    //"out vec2 textureCoords;\n"
    "uniform mat4 transformationMatrix;\n"
    "void main(){\n"
    //"  textureCoords = vec2((aPos.x+1.0)/2.0, 1-(aPos.y+1.0)/2.0);\n"
    "  gl_Position = transformationMatrix * vec4(aPos, 0.0, 1.0);\n"
    "}\n"
    ;
  const char* frag =
    "#version 430 core\n"
    //"in vec2 textureCoords;\n"
    //"uniform sampler2D guiTexture;\n"
    "out vec4  out_Color;\n"
    "uniform float alpha;\n"
    "void main() {\n"
    "  vec4 tmp_color = vec4(0.6, 0.1, 0.1, alpha);\n"
    //"  out_Color = texture(guiTexture, textureCoords);\n"
    "  out_Color = tmp_color;\n"
    "}\n"
    ;
  Shader = new OGLShader(vert, frag);

  total_time = 0;
}

void PlainGUI::Draw() {
  Shader->use();
  for (const auto& button : p_Buttons) {
    Shader->setMat4("transformationMatrix", button.transformation);
    Shader->setFloat("alpha", button.alpha);
    OGLGraphics::RenderStrip(vao_id, 4);
  }
}

void PlainGUI::UpdateUniforms(vec2 resolution, vec2 mouse_loc, float elapsed_time){
  if (!Shader)
    return;

  total_time+=elapsed_time;
  Shader->use();

  //Shader->setVec2("u_resolution", resolution);

  //Shader->setVec2("u_mouse", mouse_loc);

  //Shader->setFloat("u_time", total_time);

}

Button::Button(vec2 pos, float width_scale, float height_scale, float alpha) {
  this->pos = pos;
  this->width = width_scale;
  this->height = height_scale;
  transformation = glm::mat4(1);
  float X = (pos.x - 1.f);
  float Y = (pos.y - 1.f);
  transformation = glm::translate(transformation, vec3(X, Y, 0.0));
  transformation = glm::scale(transformation, vec3(width_scale, height_scale, 1.f));
  this->alpha = alpha;
}

}