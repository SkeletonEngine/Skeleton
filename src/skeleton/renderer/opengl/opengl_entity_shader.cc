#include "opengl_entity_shader.h"

namespace Skeleton {

const char* vert_code =
  "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;\n"
  "out vec3 v_pos;\n"
  "void main() {\n"
  "  gl_Position = vec4(a_pos, 1.0);\n"
  "  v_pos = a_pos + vec3(0.5, 0.5, 0.5);\n"
  "}\0";

const char* frag_code =
  "#version 330 core\n"
  "in vec3 v_pos;\n"
  "out vec4 o_color;\n"
  "void main() {\n"
  " o_color = vec4(v_pos, 1.0f);\n"
  "}\n\0";

EntityShader::EntityShader() {
  Compile(vert_code, frag_code);
}

}
