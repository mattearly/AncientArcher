#version 400

layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void)
{
	texCoords = position;
	vec4 pos = projectionMatrix * viewMatrix * vec4(position, 1.0); 
  gl_Position = pos.xyww;
}