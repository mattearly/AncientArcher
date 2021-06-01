#pragma once
#include <cstdint>
#include <string>
//#include <glm/ext/vector_float2.hpp>
//#include <glm/ext/vector_float3.hpp>
//#include <glm/ext/vector_float4.hpp>
//#include <glm/ext/matrix_float2x2.hpp>
//#include <glm/ext/matrix_float3x3.hpp>
//#include <glm/ext/matrix_float4x4.hpp>
//#include <glm/ext/quaternion_float.hpp>
#include <glm/glm.hpp>
namespace AA{
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef bool tf;
typedef std::string string;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::quat quat;
}