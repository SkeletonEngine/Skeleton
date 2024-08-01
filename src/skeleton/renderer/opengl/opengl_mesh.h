#pragma once

namespace Skeleton {

class Mesh {
public:
  Mesh(float* vertices, size_t vertex_count, unsigned short* indices, size_t index_count);
  ~Mesh();

public:
  void Draw();

private:
  unsigned int vao, vbo, ebo;
};

}
