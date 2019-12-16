#pragma once

#include <glad/glad.h>

class GLRenderbuffer {
 public:
  enum class Target : GLenum {
    Unbound = 0,
    Renderbuffer = GL_RENDERBUFFER
  };

  GLRenderbuffer() {
    glGenRenderbuffers(1, &id_);
  }

  ~GLRenderbuffer() {
    glGenRenderbuffers(1, &id_);
  }

  void Bind(Target target = Target::Renderbuffer) {
    target_ = target;
    glBindRenderbuffer(static_cast<GLenum>(target_), id_);
  }

  void Unbind() {
    glBindRenderbuffer(static_cast<GLenum>(target_), 0);
    target_ = Target::Unbound;
  }

  void Allocate(GLenum internalformat, GLsizei width, GLsizei height) {
    glRenderbufferStorage(static_cast<GLenum>(target_), internalformat, width, height);
  }

 private:
  GLuint id_ = 0U;
  Target target_ = Target::Unbound;

  friend class GLFramebuffer;
};
