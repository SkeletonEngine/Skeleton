#pragma once

namespace Skeleton {

class Shader {
public:
  void Bind() const;
  void Unbind() const;
  
protected:
  void Compile(const char* vert_code, const char* frag_code);
  
private:
  unsigned int shader_program;
};

}
