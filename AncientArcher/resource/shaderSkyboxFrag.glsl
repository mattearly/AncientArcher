#version 400

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube cubeMap;

void main(void){
    FragColor = texture(cubeMap, texCoords);
}