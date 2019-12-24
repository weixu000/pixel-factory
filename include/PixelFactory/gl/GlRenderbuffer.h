#pragma once

#include <glad/glad.h>

class GlRenderbuffer {
 public:
  enum class Target : GLenum {
    Unbound = 0,
    Renderbuffer = GL_RENDERBUFFER
  };

  GlRenderbuffer(GLenum internalformat, GLsizei width, GLsizei height) {
    glCreateRenderbuffers(1, &id_);
    glNamedRenderbufferStorage(id_, internalformat, width, height);
  }

  GlRenderbuffer(GlRenderbuffer &&other) noexcept: id_(other.id_) {
    other.id_ = 0U;
  }

  ~GlRenderbuffer() {
    glDeleteRenderbuffers(1, &id_);
  }

  GlRenderbuffer &operator=(GlRenderbuffer &&other) noexcept {
    assert(this != &other);
    glDeleteRenderbuffers(1, &id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void Bind(Target target = Target::Renderbuffer) {
    target_ = target;
    glBindRenderbuffer(static_cast<GLenum>(target_), id_);
  }

  void Unbind() {
    glBindRenderbuffer(static_cast<GLenum>(target_), 0);
    target_ = Target::Unbound;
  }

 private:
  GLuint id_ = 0U;
  Target target_ = Target::Unbound;
};
