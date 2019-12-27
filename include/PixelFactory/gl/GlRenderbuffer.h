#pragma once

#include <glad/glad.h>

class GlContext;

class GlRenderbuffer {
 public:
  GlRenderbuffer(GlRenderbuffer &&other) noexcept : id_(other.id_) {
    other.id_ = 0U;
  }

  ~GlRenderbuffer() { glDeleteRenderbuffers(1, &id_); }

  GlRenderbuffer &operator=(GlRenderbuffer &&other) noexcept {
    assert(this != &other);
    glDeleteRenderbuffers(1, &id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void ImmutableStorage(GLenum internalformat, GLsizei width, GLsizei height) {
    glNamedRenderbufferStorage(id_, internalformat, width, height);
  }

 private:
  GLuint id_ = 0U;

  friend class GlContext;
  explicit GlRenderbuffer(GLuint id) : id_(id) {}
};
