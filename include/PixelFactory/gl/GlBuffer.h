#pragma once

#include <glad/glad.h>

class GlBuffer {
 public:
  enum class Target : GLenum {
    Unbound = 0U,
    ArrayBuffer = GL_ARRAY_BUFFER,
    ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
  };

  GlBuffer(GLsizeiptr size, const void *data, GLbitfield flags = 0) {
    glCreateBuffers(1, &id_);
    glNamedBufferStorage(id_, size, data, flags);
  }

  GlBuffer(GlBuffer &&other) noexcept: id_(other.id_) {
    other.id_ = 0U;
  }

  ~GlBuffer() {
    glDeleteBuffers(1, &id_);
  }

  GlBuffer &operator=(GlBuffer &&other) noexcept {
    assert(this != &other);
    glDeleteBuffers(1, &id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void Bind(Target target) {
    target_ = target;
    glBindBuffer(static_cast<GLenum>(target_), id_);
  }

  void Unbind() {
    glBindBuffer(static_cast<GLenum>(target_), 0);
    target_ = Target::Unbound;
  }

 private:
  GLuint id_ = 0U;

  Target target_ = Target::Unbound;
};


