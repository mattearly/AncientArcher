// Fragment shader program
precision mediump int;
precision mediump float;

// Light model
uniform vec3 u_Light_position;
uniform vec3 u_Light_color;
uniform float u_Shininess;
uniform vec3 u_Ambient_color;

// Data coming from the vertex shader
varying vec3 v_Vertex;
varying vec4 v_Color;
varying vec3 v_Normal;

void main()
{

  vec3 to_light;
  vec3 vertex_normal;
  vec3 reflection;
  vec3 to_camera;
  float cos_angle;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 ambient_color;
  vec3 color;

  // Calculate the ambient color as a percentage of the surface color
  ambient_color = u_Ambient_color * vec3(v_Color);

  // Calculate a vector from the fragment location to the light source
  to_light = u_Light_position - v_Vertex;
  to_light = normalize( to_light );

  // The vertex's normal vector is being interpolated across the primitive
  // which can make it un-normalized. So normalize the vertex's normal vector.
  vertex_normal = normalize( v_Normal );

  // Calculate the cosine of the angle between the vertex's normal vector
  // and the vector going to the light.
  cos_angle = dot(vertex_normal, to_light);
  cos_angle = clamp(cos_angle, 0.0, 1.0);

  // Scale the color of this fragment based on its angle to the light.
  diffuse_color = vec3(v_Color) * cos_angle;

  // Calculate the reflection vector
  reflection = 2.0 * dot(vertex_normal,to_light) * vertex_normal - to_light;

  // Calculate a vector from the fragment location to the camera.
  // The camera is at the origin, so negating the vertex location gives the vector
  to_camera = -1.0 * v_Vertex;

  // Calculate the cosine of the angle between the reflection vector
  // and the vector going to the camera.
  reflection = normalize( reflection );
  to_camera = normalize( to_camera );
  cos_angle = dot(reflection, to_camera);
  cos_angle = clamp(cos_angle, 0.0, 1.0);
  cos_angle = pow(cos_angle, u_Shininess);

  // The specular color is from the light source, not the object
  if (cos_angle > 0.0) {
    specular_color = u_Light_color * cos_angle;
    diffuse_color = diffuse_color * (1.0 - cos_angle);
  } else {
    specular_color = vec3(0.0, 0.0, 0.0);
  }

  color = ambient_color + diffuse_color + specular_color;

  gl_FragColor = vec4(color, v_Color.a);
}