#include "opengl_shader.h"

#include <cstdio>

#include <glad/gl.h>

namespace Skeleton {

void Shader::Bind() const {
  glUseProgram(shader_program);
}

void Shader::Unbind() const {
  glUseProgram(0);
}

void Shader::CompileSource(const char* vert_code, const char* frag_code) {
  // vertex shader
  unsigned int vert_module = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_module, 1, &vert_code, nullptr);
  glCompileShader(vert_module);
  
  // check for shader compile errors
  int success;
  char info_log[512];
  glGetShaderiv(vert_module, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert_module, 512, nullptr, info_log);
    fprintf(stderr, "Vertex shader compilation failed:\n%s", info_log);
    return;
  }
  
  // fragment shader
  unsigned int frag_module = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_module, 1, &frag_code, nullptr);
  glCompileShader(frag_module);
  
  // check for shader compile errors
  glGetShaderiv(frag_module, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag_module, 512, nullptr, info_log);
    fprintf(stderr, "Fragment shader compilation failed:\n%s", info_log);
    return;
  }
  
  // link shaders
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vert_module);
  glAttachShader(shader_program, frag_module);
  glLinkProgram(shader_program);
  
  // check for linking errors
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
    fprintf(stderr, "Shader linking failed:\n%s", info_log);
    return;
  }
  
  glDeleteShader(vert_module);
  glDeleteShader(frag_module);
}


}
