#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec4 inColor;
layout(location = 3) in vec2 inTexUV;

out vec3 pass_Pos;
out vec3 pass_Norm;
out vec4 pass_Color;
out vec2 pass_TexUV;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main() 
{
  mat4 ViewModel = view * model;
  // Perform the model and view transformations on the vertex and pass this
  // location to the fragment shader.
  pass_Pos = vec3(ViewModel * vec4(inPos, 1.0) );
  // Perform the model and view transformations on the vertex's normal vector
  // and pass this normal vector to the fragment shader.
  pass_Norm = vec3(ViewModel * vec4(inNorm, 0.0) );
  // Pass the vertex's color to the fragment shader.
  pass_Color = inColor;
  // Pass the vertex's texture coords to the fragment shader.
  pass_TexUV = inTexUV;

  mat4 ProjViewModel = projection * view * model;
  // Transform the location of the vertex for the rest of the graphics pipeline
  gl_Position = ProjViewModel * vec4(inPos, 1.0);
}
