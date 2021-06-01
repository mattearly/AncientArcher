#version 430 core

layout(location=0)in vec3 inPos;
layout(location=1)in vec3 inNorm;
layout(location=2)in vec2 inTexUV;
layout(location=3)in vec3 inTangent;
layout(location=4)in vec3 inBiTangent;
//layout(location=5)in ivec4 inBoneIds;
//layout(location=6)in vec4 inWeights;

out vec3 pass_Pos;
out vec3 pass_Norm;
out vec2 pass_TexUV;
out vec3 pass_Tangent;
out vec3 pass_BiTangent;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

//const int MAX_BONES = 100;
//uniform mat4 finalBoneMatrices[MAX_BONES];

//const int MAX_BONE_INFLUENCE = 4;

//uniform bool isAnimated;
//uniform bool isLit;

void main()
{
  pass_Pos = inPos;
  pass_Norm = inNorm;
  pass_TexUV = inTexUV;
  pass_Tangent = inTangent;
  pass_BiTangent = inBiTangent;

  vec4 final_pos = (u_projection_matrix * u_view_matrix * u_model_matrix) * vec4(inPos, 1);

  gl_Position = final_pos;

//  if (isAnimated) {
//    vec4 totalPosition = vec4(0.0f);
//    vec3 totalNormal = vec3(0.0f);
//
//    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
//      if(inBoneIds[i] == -1)
//        continue;
//
//      if(inBoneIds[i] >= MAX_BONES){
//        totalPosition = vec4(inPos,1.0f);
//        break;
//      }
//
//      vec4 localPosition = finalBoneMatrices[inBoneIds[i]] * vec4(inPos,1.0f);
//      totalPosition += localPosition * inWeights[i];
//
//      if (isLit) {
//        vec3 localNormal = mat3(finalBoneMatrices[inBoneIds[i]]) * inNorm;
//        totalNormal += localNormal;
//      }
//    }
//    if (isLit){
//      pass_Norm = totalNormal;
//      pass_Pos = vec3(totalPosition);
//    }
//    gl_Position =  projection * view * model * totalPosition;
//  } else {  // not animated
//    if (isLit) {
//      pass_Norm = inNorm;
//      pass_Pos = inPos;
//    }
//    gl_Position =  projection * view * model * vec4(inPos, 1);
//  }


}
