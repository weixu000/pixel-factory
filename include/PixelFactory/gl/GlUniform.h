#pragma once

#include <glad/glad.h>

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T>
struct GlUniform {
  T val;
  void Set(GLuint program, GLint location) const;
};

template <>
inline void GlUniform<int>::Set(GLuint program, GLint location) const {
  glProgramUniform1i(program, location, val);
}

template <>
inline void GlUniform<float>::Set(GLuint program, GLint location) const {
  glProgramUniform1f(program, location, val);
}

template <>
inline void GlUniform<glm::vec3>::Set(GLuint program, GLint location) const {
  glProgramUniform3fv(program, location, 1, glm::value_ptr(val));
}

template <>
inline void GlUniform<glm::mat4>::Set(GLuint program, GLint location) const {
  glProgramUniformMatrix4fv(program, location, 1, GL_FALSE,
                            glm::value_ptr(val));
}

template <size_t N>
struct GlUniform<std::array<glm::mat4, N>> {
  std::array<glm::mat4, N> val;
  void Set(GLuint program, GLint location) const {
    glProgramUniformMatrix4fv(program, location, N, GL_FALSE,
                              reinterpret_cast<const GLfloat*>(val.data()));
  }
};