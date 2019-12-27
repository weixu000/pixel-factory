#pragma once

#include <PixelFactory/GL/GlTexture.h>

class GlContext;

class GlTextureCubemap : public GlTexture {
 public:
  void ImmutableStorage(GLsizei levels, GLenum internalformat, GLsizei size) {
    glTextureStorage2D(id_, levels, internalformat, size, size);
    size_ = size;
  }

  [[nodiscard]] GLsizei Size() const { return size_; }

 private:
  GLsizei size_ = 0;

  friend class GlContext;
  explicit GlTextureCubemap(GLuint id) : GlTexture(TextureTarget::TexutureCubeMap, id) {}
};
