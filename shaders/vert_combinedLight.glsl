// Vertex Shader
precision mediump int;
precision mediump float;

// Scene transformations
uniform mat4 u_PVM_transform; // Projection, view, model transform
uniform mat4 u_VM_transform;  // View, model transform

// Light model
uniform vec3 u_Light_position;
uniform vec3 u_Light_color;
uniform float u_Shininess;
uniform vec3 u_Ambient_color;

// Original model data
attribute vec3 a_Vertex;
attribute vec3 a_Color;
attribute vec3 a_Vertex_normal;

// Data (to be interpolated) that is passed on to the fragment shader
varying vec3 v_Vertex;
varying vec4 v_Color;
varying vec3 v_Normal;

void main() {

  // Perform the model and view transformations on the vertex and pass this
  // location to the fragment shader.
  v_Vertex = vec3( u_VM_transform * vec4(a_Vertex, 1.0) );

  // Perform the model and view transformations on the vertex's normal vector
  // and pass this normal vector to the fragment shader.
  v_Normal = vec3( u_VM_transform * vec4(a_Vertex_normal, 0.0) );

  // Pass the vertex's color to the fragment shader.
  v_Color = vec4(a_Color, 1.0);

  // Transform the location of the vertex for the rest of the graphics pipeline
  gl_Position = u_PVM_transform * vec4(a_Vertex, 1.0);
}