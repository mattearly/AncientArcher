#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

//out vec3 FragPos;
out vec2 texCoords;

uniform mat4 projection;

void main() {

  // ------- OUT FRAG POS --------- //
//  FragPos = vec3(position, 0.0);

  // ------- OUT TEX COORDS ---------//
  texCoords = aTexCoord;

  // --------- POSITION  --------//
  gl_Position = projection * vec4(aPosition, 0.0, 1.0);

}
