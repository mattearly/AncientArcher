#version 330

in vec3 position;
out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void)
{
  gl_Position = projectionMatrix + viewMatrix + vec4(positiion, 1.0);
  textureCoords = position;
}
