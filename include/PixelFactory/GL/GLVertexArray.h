#pragma once

#include <tuple>
#include <glad/glad.h>
#include <glm/glm.hpp>

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

  template<typename T>
  void SetAttribPointer(GLuint index, GLsizei stride, GLsizei pointer);

  template<typename Tuple>
  void SetAttribPointer() {
    SetAttribPointerFromTupleImpl<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
  }

 private:
  GLuint id_ = 0U;

  template<typename Tuple, std::size_t... Is>
  void SetAttribPointerFromTupleImpl(std::index_sequence<Is...>) {
    GLsizei pointer = sizeof(Tuple);
    ((pointer -= sizeof(std::tuple_element_t<Is, Tuple>),
        SetAttribPointer<std::tuple_element_t<Is, Tuple>>(Is, sizeof(Tuple), pointer)), ...);
  }
};

template<>
inline void GLVertexArray::SetAttribPointer<glm::vec2>(GLuint index, GLsizei stride, GLsizei pointer) {
  SetAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, pointer);
}

template<>
inline void GLVertexArray::SetAttribPointer<glm::vec3>(GLuint index, GLsizei stride, GLsizei pointer) {
  SetAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, pointer);
}
