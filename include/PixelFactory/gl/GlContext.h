#pragma once

#include <PixelFactory/GL/GlBuffer.h>
#include <PixelFactory/GL/GlFramebuffer.h>
#include <PixelFactory/GL/GlRenderbuffer.h>
#include <PixelFactory/GL/GlTexture2D.h>
#include <PixelFactory/GL/GlTextureCubemap.h>
#include <PixelFactory/GL/GlVertexArray.h>

class GlContext {
public:
  GlContext();

  GlContext(const GlContext &) = delete;
  GlContext(GlContext &&) = default;
  GlContext &operator=(const GlContext &) = delete;
  GlContext &operator=(GlContext &&) = default;

  void Clear(bool color, bool depth);
  void Viewport(int x, int y, int width, int height);

  GlBuffer CreateBuffer();
  GlRenderbuffer CreateRenderbuffer();
  GlTexture2D CreateTexture2D();
  GlTextureCubemap CreateTextureCubemap();
  GlFramebuffer CreateFramebuffer();
  GlVertexArray CreateVertexArray();

  void Bind(BufferTarget target, GlBuffer &buffer);
  void Bind(GlRenderbuffer &renderbuffer);
  void Bind(GlTexture &texture);
  void Bind(FramebufferTarget target, GlFramebuffer &framebuffer);
  void Bind(GlVertexArray &vertex_array);

  GlBuffer &NullBuffer() { return null_buffer_; }
  GlRenderbuffer &NullRenderbuffer() { return null_renderbuffer_; }
  GlTexture2D &NullTexture2D() { return null_texture2d_; }
  GlTextureCubemap &NullTextureCubemap() { return null_texture_cubemap_; }
  GlFramebuffer &DefaultFramebuffer() { return default_framebuffer_; }
  GlVertexArray &NullVertexArray() { return null_vertex_array_; }

 private:
  // Objects with name 0
  GlBuffer null_buffer_;
  GlRenderbuffer null_renderbuffer_;
  GlTexture2D null_texture2d_;
  GlTextureCubemap null_texture_cubemap_;
  GlFramebuffer default_framebuffer_; // Default framebuffer
  GlVertexArray null_vertex_array_;
};
