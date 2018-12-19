// Geometry Shader

#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 gNormal;

// You will need to pass your untransformed positions in from the vertex shader
in vec3 vPosition[];

uniform mat3 normalMatrix;

void main() {
  vec3 side2 = vPosition[2] - vPosition[0];
  vec3 side0 = vPosition[1] - vPosition[0];
  vec3 facetNormal = normalize(normalMatrix * cross(side0, side2));

  gNormal = facetNormal;
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gNormal = facetNormal;
  gl_Position = gl_in[1].gl_Position;
  EmitVertex();

  gNormal = facetNormal;
  gl_Position = gl_in[2].gl_Position;
  EmitVertex();

  EndPrimitive();
}