#version 330

in vec2 texCoords;

out vec4 fragColor;

uniform vec3 colour;
uniform sampler2D fontAtlas;

void main(void){

	fragColor = vec4(colour, texture(fontAtlas, texCoords).a);

}