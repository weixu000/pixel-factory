#pragma once

#include <glad/glad.h>

class GLTexture {
 public:
  enum class Target : GLenum {
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    TexutureCubeMap = GL_TEXTURE_CUBE_MAP,
  };

  explicit GLTexture(Target target) : target_(target) {
    glGenTextures(1, &id_);
  }

  virtual ~GLTexture() {
    glDeleteTextures(1, &id_);
  }

  void Bind() { glBindTexture(static_cast<GLenum>(target_), id_); }

  void Unbind() { glBindTexture(static_cast<GLenum>(target_), 0); }

  void SetFilter(GLint min, GLint max) {
    glTexParameteri(static_cast<GLenum>(target_), GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(static_cast<GLenum>(target_), GL_TEXTURE_MAG_FILTER, max);
  }

  void SetWrap(GLint s, GLint t) {
    glTexParameteri(static_cast<GLenum>(target_), GL_TEXTURE_WRAP_S, s);
    glTexParameteri(static_cast<GLenum>(target_), GL_TEXTURE_WRAP_T, t);
  }
 protected:
  const Target target_;
  GLuint id_;
};
