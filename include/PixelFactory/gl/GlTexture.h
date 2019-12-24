#pragma once

#include <glad/glad.h>

class GlTexture {
 public:
  enum class Target : GLenum {
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    TexutureCubeMap = GL_TEXTURE_CUBE_MAP,
  };

  explicit GlTexture(Target target) : target_(target) {
    glCreateTextures(static_cast<GLenum>(target_), 1, &id_);
  }

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

  [[nodiscard]] GLuint Id() const { return id_; }

  void Bind() { glBindTexture(static_cast<GLenum>(target_), id_); }

  void Unbind() { glBindTexture(static_cast<GLenum>(target_), 0); }

  void SetFilter(GLint min, GLint max) {
    glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, min);
    glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, max);
  }

  void SetWrap(GLint s, GLint t) {
    glTextureParameteri(id_, GL_TEXTURE_WRAP_S, s);
    glTextureParameteri(id_, GL_TEXTURE_WRAP_T, t);
  }
 protected:
  const Target target_;
  GLuint id_;
};
