#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers from texture bank
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5; 
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;
uniform sampler2D texture14;
uniform sampler2D texture15;

// texture samplers switcher for texture bank
uniform uint texnum;

void main() {

  // texture bank switch
  switch (texnum) {
  case 0: FragColor = texture(texture0, TexCoord); break;
  case 1: FragColor = texture(texture1, TexCoord); break;
  case 2: FragColor = texture(texture2, TexCoord); break;
  case 3: FragColor = texture(texture3, TexCoord); break;
  case 4: FragColor = texture(texture4, TexCoord); break;
  case 5: FragColor = texture(texture5, TexCoord); break;
  case 6: FragColor = texture(texture6, TexCoord); break; 
  case 7: FragColor = texture(texture7, TexCoord); break; 
  case 8: FragColor = texture(texture8, TexCoord); break; 
  case 9: FragColor = texture(texture9, TexCoord); break; 
  case 10: FragColor = texture(texture10, TexCoord); break; 
  case 11: FragColor = texture(texture12, TexCoord); break; 
  case 13: FragColor = texture(texture13, TexCoord); break; 
  case 14: FragColor = texture(texture14, TexCoord); break; 
  case 15: FragColor = texture(texture15, TexCoord); break; 
  }


  // linearly interpolate between both textures (80% first, 20% second)
//FragColor = mix(texture(texture001, TexCoord), texture(texture002, TexCoord), 0.2);
}