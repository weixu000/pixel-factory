#pragma once

#include <PixelFactory/gl/GlTexture.h>

class GlTextureCubemap : public GlTexture {
 public:
  GlTextureCubemap(GLsizei levels, GLenum internalformat, GLsizei size)
      : GlTexture(Target::TexutureCubeMap),
        size_(size) {
    glTextureStorage2D(id_, levels, internalformat, size, size);
  }

  const GLsizei size_;
};
