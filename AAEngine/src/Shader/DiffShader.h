#pragma once
namespace AA
{
const char* diff_vert_src =
"#version 430 core\n"
"layout(location=0)in vec3 inPos;\n"
"layout(location=1)in vec2 inTexUV;\n"
"out vec2 pass_TexUV;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"void main(){\n"
"  pass_TexUV = inTexUV;\n"
"  gl_Position = projection * view * model * vec4(inPos,1);\n"
"}\n"
;
const char* diff_frag_src =
"#version 430 core\n"
"in vec2 pass_TexUV;\n"
"out vec4 out_Color;\n"
"struct Material {\n"
"  sampler2D Albedo;\n"
"};\n"
"uniform Material material;\n"
"void main(){\n"
"  out_Color = texture(material.Albedo, pass_TexUV);\n"
"}\n"
;
}