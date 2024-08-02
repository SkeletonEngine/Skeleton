#pragma once

namespace Skeleton {

class Framebuffer {
public:
  Framebuffer(int width, int height);
  ~Framebuffer();

public:
  void Resize(int width, int height);

public:
  void Bind() const;
  void Unbind() const;

public:
  int GetWidth() const { return width; }
  int GetHeight() const { return height; }

public:
  unsigned int GetColorBufferHandle() const { return color_buffer; }

private:
  unsigned int framebuffer;
  unsigned int color_buffer;
  unsigned int render_buffer;

  int width, height;
};

}
