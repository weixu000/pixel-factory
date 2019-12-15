#pragma once

#include <glad/glad.h>

class GLVertexArray {
 public:
  GLVertexArray() {
    glGenVertexArrays(1, &id_);
  }

  ~GLVertexArray() {
    glDeleteVertexArrays(1, &id_);
  }

  void Bind() { glBindVertexArray(id_); }

  static void Unbind() { glBindVertexArray(0); }

  void SetAttribPointer(GLuint index,
                        GLint size, GLenum type, GLboolean normalized,
                        GLsizei stride, GLsizei pointer = 0) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized,
                          stride, reinterpret_cast<const void *>(pointer));
  }

 private:
  GLuint id_ = 0U;
};


