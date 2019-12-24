#pragma once

#include <tuple>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/gl/GlBuffer.h>

class GlVertexArray {
 public:
  GlVertexArray() {
    glCreateVertexArrays(1, &id_);
  }

  GlVertexArray(GlVertexArray &&other) noexcept: id_(other.id_) {
    other.id_ = 0U;
  }

  ~GlVertexArray() {
    glDeleteVertexArrays(1, &id_);
  }

  GlVertexArray &operator=(GlVertexArray &&other) noexcept {
    assert(this != &other);
    glDeleteVertexArrays(1, &id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void Bind() { glBindVertexArray(id_); }

  static void Unbind() { glBindVertexArray(0); }

  void BindElementBuffer(const GlBuffer &buffer) {
    glVertexArrayElementBuffer(id_, buffer.Id());
  }

  void BindVertexBuffer(GLuint bindingindex, const GlBuffer &buffer,
                        GLintptr offset, GLsizei stride) {
    glVertexArrayVertexBuffer(id_, bindingindex, buffer.Id(), offset, stride);
  }

  void AssociateAttrib(GLuint attribindex, GLuint bindingindex) {
    glVertexArrayAttribBinding(id_, attribindex, bindingindex);
  }

  void SetAttribFormat(GLuint attribindex,
                       GLint size, GLenum type, GLboolean normalized,
                       GLuint relativeoffset) {
    glEnableVertexArrayAttrib(id_, attribindex);
    glVertexArrayAttribFormat(id_, attribindex, size, type, normalized, relativeoffset);
  }

  template<typename T>
  void SetAttribFormat(GLuint attribindex, GLuint relativeoffset);

  template<typename Tuple>
  void SetAttribFormat(GLuint bindingindex) {
    SetAttribFormatFromTupleImpl<Tuple>(bindingindex, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
  }

 private:
  GLuint id_ = 0U;

  template<typename Tuple, std::size_t... Is>
  void SetAttribFormatFromTupleImpl(GLuint bindingindex, std::index_sequence<Is...>) {
    (AssociateAttrib(Is, bindingindex),...);
    GLuint offset = sizeof(Tuple);
    ((offset -= sizeof(std::tuple_element_t<Is, Tuple>),
        SetAttribFormat<std::tuple_element_t<Is, Tuple>>(Is, offset)), ...);
  }
};

template<>
inline void GlVertexArray::SetAttribFormat<glm::vec2>(GLuint attribindex, GLuint relativeoffset) {
  SetAttribFormat(attribindex, 2, GL_FLOAT, GL_FALSE, relativeoffset);
}

template<>
inline void GlVertexArray::SetAttribFormat<glm::vec3>(GLuint attribindex, GLuint relativeoffset) {
  SetAttribFormat(attribindex, 3, GL_FLOAT, GL_FALSE, relativeoffset);
}
