#include "PlainGUI.h"
#include <vector>
#include "../Renderer/ModelLoader.h"
#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
#include "../../include/AncientArcher/AncientArcher.h"
#include "../Utility/QueryShader.h"
namespace AA {

PlainGUI::PlainGUI() {
  InitShader();
}

PlainGUI::~PlainGUI() {
  delete Shader;
  Shader = NULL;
}

void PlainGUI::AddButton(vec2 pos, f32 width_scale, f32 height_scale, vec3 color, f32 alpha) {
  p_Buttons.emplace_back(pos, width_scale, height_scale, color, alpha);
}

void PlainGUI::AddButton(vec2 pos, f32 width_scale, f32 height_scale, f32 alpha, const char* texture_path) {
  p_Buttons.emplace_back(pos, width_scale, height_scale, alpha, texture_path);
}

void PlainGUI::ShowInterface() {
  p_HideAll = false;
}

void PlainGUI::HideInterface() {
  p_HideAll = true;
}

//void PlainGUI::AddSlider(Slider slider, void(*resulthandler)(f32 result)) {
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
    "out vec2 pass_pos;\n"
    "uniform mat4 u_transform;\n"
    "void main(){\n"
    "  pass_pos = aPos;\n"
    "  gl_Position = u_transform * vec4(aPos, 0.0, 1.0);\n"
    "}\n"
    ;
  const char* frag =
    "#version 430 core\n"
    "in vec2 pass_pos;\n"
    "out vec4  out_Color;\n"

    "uniform int u_mouseoverlapping;\n"
    "uniform int u_istextured;\n"
    "uniform sampler2D u_texture;\n"
    "uniform float u_alpha;\n"
    "uniform vec3 u_color;\n"

    "void main() {\n"
    "  float tmp_alpha = u_alpha;\n"
    "  if (u_mouseoverlapping == 1) {\n"
    "    tmp_alpha = 1.0;\n"
    "  }\n"
    "  if (u_istextured == 1) {\n"
    "    vec2 texCoords = vec2((pass_pos.x+1.0)/2.0, 1-(pass_pos.y+1.0)/2.0);\n"
    "    out_Color = vec4(texture(u_texture, texCoords).rgb, tmp_alpha);\n"
    "  } else {\n"
    "    out_Color = vec4(u_color, tmp_alpha);\n"
    "  }\n"
    "}\n"
    ;
  Shader = new OGLShader(vert, frag);

#if _DEBUG
  QueryInputAttribs(Shader->GetID());
  QueryUniforms(Shader->GetID());
#endif

  total_time = 0;
}

void PlainGUI::Draw() {
  if (p_HideAll)
    return;

  Shader->use();
  for (const auto& button : p_Buttons) {
    Shader->setMat4("u_transform", button.transformation);

    Shader->setBool("u_istextured", (button.texture_id != 0) ? true : false);

    if (button.texture_id != 0) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, button.texture_id);
    }
    Shader->setInt("u_texture", 0);
    Shader->setVec3("u_color", button.color);

    Shader->setFloat("u_alpha", button.alpha);
    Shader->setBool("u_mouseoverlapping", (button.overlapped) ? true : false);

    OGLGraphics::RenderStrip(vao_id, 4);
  }

}

void PlainGUI::UpdateMouseOverlap(vec2 mouse_loc) {
  f64 halfwidth = GetWindowWidth() / 2.0;
  f64 halfheight = GetWindowHeight() / 2.0;
  f64 mouse_loc_x_neg1to1 = ((mouse_loc.x - halfwidth) / halfwidth);
  f64 mouse_loc_y_neg1to1 = -((mouse_loc.y - halfheight) / halfheight);
  //std::cout << "converted pos: " << mouse_loc_x_neg1to1 << " " << mouse_loc_y_neg1to1 << '\n';
  for (auto& button : p_Buttons) {
    if (
      mouse_loc_x_neg1to1 > button.pos.x - (button.width * halfwidth) / halfwidth
      &&
      mouse_loc_x_neg1to1 < button.pos.x + (button.width * halfwidth) / halfwidth
      &&
      mouse_loc_y_neg1to1 > button.pos.y - (button.height * halfheight) / halfheight
      &&
      mouse_loc_y_neg1to1 < button.pos.y + (button.height * halfheight) / halfheight
      )
      button.overlapped = true;
    else {
      button.overlapped = false;
    }
  }
}

void PlainGUI::UpdateMouseLoc(vec2 mouse_loc) {
  UpdateMouseOverlap(mouse_loc);
}

void PlainGUI::UpdateUniforms(vec2 resolution, vec2 mouse_loc, f32 elapsed_time) {
  if (!Shader)
    return;

  //total_time += elapsed_time;

  //Shader->use();

  UpdateMouseOverlap(mouse_loc);

  //Shader->setVec2("u_resolution", resolution);

  //Shader->setVec2("mouse_loc", mouse_loc);

  //Shader->setFloat("u_time", total_time);

}

Button::Button(vec2 pos, f32 width_scale, f32 height_scale, vec3 color, f32 alpha) {
  this->pos = pos;
  this->width = width_scale;
  this->height = height_scale;
  transformation = glm::mat4(1);
  transformation = glm::translate(transformation, vec3(pos.x, pos.y, 0.0));
  transformation = glm::scale(transformation, vec3(height, width, 1.f));
  this->color = color;
  this->alpha = alpha;
  this->texture_id = -1;
}

Button::Button(vec2 pos, f32 width_scale, f32 height_scale, f32 alpha, const char* texture_path) {
  this->pos = pos;
  this->width = width_scale;
  this->height = height_scale;
  transformation = glm::mat4(1);
  transformation = glm::translate(transformation, vec3(pos.x, pos.y, 0.0));
  transformation = glm::scale(transformation, vec3(height, width, 1.f));
  this->color = vec3(.7, .1, .7);
  this->alpha = alpha;
  this->texture_id = TextureLoader::LoadTexture(texture_path);
}
}