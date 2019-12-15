#pragma once

#include <glad/glad.h>

enum class GLBufferTarget : GLenum {
  Unbound = 0U,
  ArrayBuffer = GL_ARRAY_BUFFER,
  ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
};

class GLBuffer {
 public:
  GLBuffer() {
    glGenBuffers(1, &id_);
  }

  ~GLBuffer() {
    glDeleteBuffers(1, &id_);
  }

  void Upload(GLsizeiptr size, const void *data, GLenum usage = GL_STATIC_DRAW) {
    glBufferData(static_cast<GLenum>(target_), size, data, usage);
  }

  void Bind(GLBufferTarget target) {
    target_ = target;
    glBindBuffer(static_cast<GLenum>(target_), id_);
  }

  void Unbind() {
    glBindBuffer(static_cast<GLenum>(target_), 0);
  }

 private:
  GLuint id_ = 0U;

  GLBufferTarget target_ = GLBufferTarget::Unbound;
};


