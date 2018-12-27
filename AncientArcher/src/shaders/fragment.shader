#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec2 TexCoord;
in vec3 Normal;

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
uniform int texnum;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() {

  FragColor = vec4(0.5, 0.5, 0.5, 1.0);

   //texture bank switch
  //switch (texnum) {
  //case 0: FragColor = texture(texture0, TexCoord); break;
  //case 1: FragColor = texture(texture1, TexCoord); break;
  //case 2: FragColor = texture(texture2, TexCoord); break;
  //case 3: FragColor = texture(texture3, TexCoord); break;
  //case 4: FragColor = texture(texture4, TexCoord); break;
  //case 5: FragColor = texture(texture5, TexCoord); break;
  //case 6: FragColor = texture(texture6, TexCoord); break; 
  //case 7: FragColor = texture(texture7, TexCoord); break; 
  //case 8: FragColor = texture(texture8, TexCoord); break; 
  //case 9: FragColor = texture(texture9, TexCoord); break; 
  //case 10: FragColor = texture(texture10, TexCoord); break; 
  //case 11: FragColor = texture(texture11, TexCoord); break; 
  //case 12: FragColor = texture(texture12, TexCoord); break; 
  //case 13: FragColor = texture(texture13, TexCoord); break; 
  //case 14: FragColor = texture(texture14, TexCoord); break; 
  //case 15: FragColor = texture(texture15, TexCoord); break; 
  //} 

  vec3 color;

  switch (texnum) {
  case 0: color = texture(texture0, TexCoord).rgb; break;
  case 1: color = texture(texture1, TexCoord).rgb; break;
  case 2: color = texture(texture2, TexCoord).rgb; break;
  case 3: color = texture(texture3, TexCoord).rgb; break;
  case 4: color = texture(texture4, TexCoord).rgb; break;
  case 5: color = texture(texture5, TexCoord).rgb; break;
  case 6: color = texture(texture6, TexCoord).rgb; break;
  case 7: color = texture(texture7, TexCoord).rgb; break;
  case 8: color = texture(texture8, TexCoord).rgb; break;
  case 9: color = texture(texture9, TexCoord).rgb; break;
  case 10: color = texture(texture10, TexCoord).rgb; break; 
  case 11: color = texture(texture11, TexCoord).rgb; break; 
  case 12: color = texture(texture12, TexCoord).rgb; break; 
  case 13: color = texture(texture13, TexCoord).rgb; break; 
  case 14: color = texture(texture14, TexCoord).rgb; break; 
  case 15: color = texture(texture15, TexCoord).rgb; break; 
  }

  //vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;

  // ambient
  vec3 ambient = 0.20 * color;
  
  // diffuse
  vec3 lightDir = normalize(lightPosition - FragPosition);
  vec3 normal = normalize(Normal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color;

  // specular
  vec3 viewDir = normalize(viewPosition - FragPosition);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.5;

  vec3 halfwayDir = normalize(lightDir + viewDir);   //blinn
  spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
  
  vec3 specular = vec3(0.3) * spec; // assuming bright white light color
  FragColor = vec4(ambient + diffuse + specular, 1.0);

}

// lighting reference: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/1.advanced_lighting/1.advanced_lighting.fs
