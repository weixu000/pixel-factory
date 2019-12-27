#pragma once

#include <glad/glad.h>

enum class TextureTarget : GLenum {
  Texture1D = GL_TEXTURE_1D,
  Texture2D = GL_TEXTURE_2D,
  TexutureCubeMap = GL_TEXTURE_CUBE_MAP,
};

class GlTexture {
 public:
  GlTexture(GlTexture &&other) noexcept: target_(other.target_), id_(other.id_) {
    other.id_ = 0U;
  }

  virtual ~GlTexture() {
    glDeleteTextures(1, &id_);
  }

  GlTexture &operator=(GlTexture &&other) noexcept {
    assert(this != &other);
    glDeleteTextures(1, &id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] TextureTarget Target() const { return target_; }

  [[nodiscard]] GLuint Id() const { return id_; }

  void SetFilter(GLint min, GLint max) {
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, min);
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, max);
  }

  void SetWrap(GLint s, GLint t) {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, s);
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, t);
  }

 protected:
  const TextureTarget target_;
  GLuint id_;

  explicit GlTexture(TextureTarget target, GLuint id) : target_(target), id_(id) {}
};
