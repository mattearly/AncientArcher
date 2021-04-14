#include "PlainGUI.h"
#include <vector>
#include "../Renderer/OpenGL/OGLGraphics.h"
namespace AA {
PlainGUI::PlainGUI() {
  InitShader();
}
void PlainGUI::AddSlider(Slider slider, void(*resulthandler)(float result)) {
  if (slider.bot_left_loc.x < -1.f || slider.bot_left_loc.y < -1.f)
    throw("beyond -1");
  if (slider.bot_left_loc.x > 1.f || slider.bot_left_loc.y > 1.f)
    throw("beyond 1");

  std::vector<vec3> points;
  points.push_back(vec3(slider.bot_left_loc.x, slider.bot_left_loc.y, 0.f));  //bot left  ind:0
  if (slider.horizontal) {
    points.push_back(vec3(slider.bot_left_loc.x,                     slider.bot_left_loc.y + slider.bar_thickness, 0.f));  //top left  1
    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_length, slider.bot_left_loc.y + slider.bar_thickness, 0.f)); //top right  2
    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_length, slider.bot_left_loc.y,                        0.f));  // bot right  3
  } else  //vertical
  {
    points.push_back(vec3(slider.bot_left_loc.x,                        slider.bot_left_loc.y + slider.bar_length, 0.f));  //top left  1
    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_thickness, slider.bot_left_loc.y + slider.bar_length, 0.f));  //top right  2
    points.push_back(vec3(slider.bot_left_loc.x + slider.bar_thickness, slider.bot_left_loc.y,                     0.f));  // bot right  3
  }

  std::vector<u32> indices = { 0, 1, 2, 2, 3, 0 };

  vao_id = OGLGraphics::UploadVerts(points, indices);
  ind_size = sizeof(indices) / sizeof(indices[0]);  // should be 6
}

void PlainGUI::InitShader() {
  if (Shader)
    return;

  const char* vert =
    "#version 430 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    //"uniform vec3 color;\n";
    //"out pass_color;\n"
    "void main(){\n"
    //"  pass_color = color;\n"
    "  vec4 pos = vec4(aPos, 1.0);\n"
    "  gl_Position = pos.xywz;\n"
    "}\n"
    ;
  const char* frag =
    "#version 430 core\n"
    //"in vec3 pass_color;\n"
    "uniform vec2 u_resolution;\n"
    "uniform vec2 u_mouse;\n"
    "uniform float u_time;\n"
    "float plot (vec2 st) {\n"
    "  return smoothstep(0.02, 0.0, abs(st.y - st.x));\n"
    "}\n"
    //"out vec4 FragColor;\n"
    "void main() {\n"
    "  vec2 st = gl_FragCoord.xy / u_resolution;\n"
    "  vec3 color = vec3(y);\n"
    "  float pct = plot(st);\n"
    "  color = (1.0-pct)*color+pct*vec3(0.0,1.0,0.0);\n"
    "  gl_FragColor = vec4(color,1.0);\n"
    "}\n"
    ;
  Shader = new OGLShader(vert, frag);

}

void PlainGUI::Draw() {
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

  glBindVertexArray(mSkyboxVAO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  skyboxShader->use();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

}