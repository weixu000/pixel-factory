#pragma once

#include <glad/glad.h>

class GLBuffer {
 public:
  enum class Target : GLenum {
    Unbound = 0U,
    ArrayBuffer = GL_ARRAY_BUFFER,
    ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
  };

  GLBuffer() {
    glGenBuffers(1, &id_);
  }

  ~GLBuffer() {
    glDeleteBuffers(1, &id_);
  }

  void Upload(GLsizeiptr size, const void *data, GLenum usage = GL_STATIC_DRAW) {
    glBufferData(static_cast<GLenum>(target_), size, data, usage);
  }

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


