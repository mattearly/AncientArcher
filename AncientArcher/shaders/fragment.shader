#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture001;
uniform sampler2D texture002;

void main()
{
//    FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(texture001, TexCoord), texture(texture002, TexCoord), 0.2);
    //FragColor = mix(texture(texture001, TexCoord), texture(texture002, TexCoord), 0.2) * vec4(ourColor, 1.0);
    //FragColor = mix(texture(texture001, TexCoord), texture(texture002, TexCoord), 0.2) * ourColor;
//    FragColor = texture(texture1, TexCoord);
}
