#pragma once

#include <cassert>
#include <glad/glad.h>

class GlContext;

enum class BufferTarget : GLenum {
  ArrayBuffer = GL_ARRAY_BUFFER,
  ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
};

class GlBuffer {
 public:
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

  void ImmutableStorage(GLsizeiptr size, const void *data, GLbitfield flags = 0) {
    glNamedBufferStorage(id_, size, data, flags);
  }

 private:
  GLuint id_ = 0U;

  friend class GlContext;
  explicit GlBuffer(GLuint id) : id_(id) {}
};


