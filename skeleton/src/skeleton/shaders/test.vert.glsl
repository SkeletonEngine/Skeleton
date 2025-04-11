#version 450

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec3 a_color;

layout (location = 0) out vec3 v_color;

layout (binding = 0) uniform MvpUniformBuffer {
  mat4 model;
  mat4 view;
  mat4 proj;
} u_mvp;

void main() {
  v_color = a_color;
  gl_Position = u_mvp.proj * u_mvp.view * u_mvp.model * vec4(a_pos, 0.0, 1.0);
}
