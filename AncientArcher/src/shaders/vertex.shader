#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

  FragPos = vec3(model * vec4(aPosition, 1.0));
  TexCoords = aTexCoord;
  //Normal = aNormal;
  Normal = mat3(transpose(inverse(model))) * aNormal;   // to handle normals on oddly shapped transforms
  
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
}

// lighting reference: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/1.advanced_lighting/1.advanced_lighting.vs
