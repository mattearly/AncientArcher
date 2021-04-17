#include "PlainGUI.h"
#include <vector>
#include "../Renderer/OpenGL/OGLGraphics.h"
#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
#include "../../include/AncientArcher/AncientArcher.h"
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
    //"uinform vec2 u_resolution;\n"
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
    "uniform int mouseOverlapping;\n"
    "uniform float alpha;\n"
    //"uinform vec2 u_resolution;\n"
    "void main() {\n"
    //"  out_Color = texture(guiTexture, textureCoords);\n"
    "  float tmp_alpha = alpha;\n"
    "  if (mouseOverlapping == 1) {\n"
    "    tmp_alpha = 1.0;\n"
    "  }\n"
    "  vec4 tmp_color = vec4(0.6, 0.1, 0.1, tmp_alpha);\n"
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
    Shader->setInt("mouseOverlapping", (button.overlapped) ? 1 : 0);
    OGLGraphics::RenderStrip(vao_id, 4);
  }
}

void PlainGUI::UpdateMouseOverlap(vec2 mouse_loc) {
  float halfwidth = GetWindowWidth() / 2.0;
  float halfheight = GetWindowHeight() / 2.0;
  float mouse_loc_x_neg1to1 = ((mouse_loc.x - halfwidth) / halfwidth);
  float mouse_loc_y_neg1to1 = -((mouse_loc.y - halfheight) / halfheight);
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

void PlainGUI::UpdateUniforms(vec2 resolution, vec2 mouse_loc, float elapsed_time) {
  if (!Shader)
    return;

  //total_time += elapsed_time;

  //Shader->use();

  UpdateMouseOverlap(mouse_loc);

  //Shader->setVec2("u_resolution", resolution);

  //Shader->setVec2("mouse_loc", mouse_loc);

  //Shader->setFloat("u_time", total_time);

}

Button::Button(vec2 pos, float width_scale, float height_scale, float alpha) {
  this->pos = pos;
  this->width = width_scale;
  this->height = height_scale;

  transformation = glm::mat4(1);

  //std::cout << "button pos X: " << pos.x << " Y: " << pos.y << '\n';

  transformation = glm::translate(transformation, vec3(pos.x, pos.y, 0.0));

  transformation = glm::scale(transformation, vec3(height, width, 1.f));

  this->alpha = alpha;
}

}