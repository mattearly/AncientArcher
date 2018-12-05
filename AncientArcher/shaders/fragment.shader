#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture001;
uniform sampler2D texture002;

void main() {
  // linearly interpolate between both textures (80% container, 20% awesomeface)
  FragColor = mix(texture(texture001, TexCoord), texture(texture002, TexCoord), 0.2);
}