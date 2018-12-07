#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture001;  //crate
uniform sampler2D texture002;  //awesomeface
uniform sampler2D texture003;  //the radial
uniform sampler2D texture004;  //grass
uniform sampler2D texture005;  //stone
uniform sampler2D texture006;  //mathematics
uniform sampler2D texture007;  //meme

// texture activator
uniform bool tex1;
uniform bool tex2;
uniform bool tex3;
uniform bool tex4;
uniform bool tex5;
uniform bool tex6;
uniform bool tex7;

void main() {
  // linearly interpolate between both textures (80% first, 20% second)
  //FragColor = mix(texture(texture001, TexCoord), texture(texture002, TexCoord), 0.2);


  if (tex1) {
    FragColor = texture(texture001, TexCoord);
  } 
  
  if (tex2) {
    FragColor = texture(texture002, TexCoord);
  } 
  
  if (tex3) {
    FragColor = texture(texture003, TexCoord);
  }

  if (tex4) {
    FragColor = texture(texture004, TexCoord);
  }
  
  if (tex5) {
    FragColor = texture(texture005, TexCoord);
  }  
  
  if (tex6) {
    FragColor = texture(texture006, TexCoord);
  }  

  if (tex7) {
    FragColor = texture(texture007, TexCoord);
  }

}