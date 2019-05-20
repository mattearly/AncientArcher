#version 330

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoords;

uniform vec2 translation;

void main(void){

	gl_Position = vec4(aPosition + translation * vec2(2.0, -2.0), 0.0, 1.0);
	texCoords = aTexCoord;

}