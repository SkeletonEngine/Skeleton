#version 450

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec3 a_color;

layout (location = 0) out vec3 v_color;

layout (set = 0, binding = 0) uniform CameraUniformBuffer {
  mat4 matrix;
} u_camera;

layout (set = 0, binding = 1) uniform ModelUniformBuffer {
  mat4 matrix;
} u_model;

void main() {
  v_color = a_color;
  gl_Position = u_camera.matrix * u_model.matrix * vec4(a_pos, 0.0, 1.0);
}
