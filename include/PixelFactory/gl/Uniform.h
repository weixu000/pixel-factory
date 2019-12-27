#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <memory>
#include <string>
#include <utility>

template <typename T>
struct Uniform {
  std::string name;
  T val;
  void Set(GLint location) const;
};

template <>
inline void Uniform<int>::Set(GLint location) const {
  glUniform1i(location, val);
}

template <>
inline void Uniform<float>::Set(GLint location) const {
  glUniform1f(location, val);
}

template <>
inline void Uniform<glm::vec3>::Set(GLint location) const {
  glUniform3fv(location, 1, glm::value_ptr(val));
}

template <>
inline void Uniform<glm::mat4>::Set(GLint location) const {
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
}

template <size_t N>
struct Uniform<std::array<glm::mat4, N>> {
  std::string name;
  std::array<glm::mat4, N> val;
  void Set(GLint location) const {
    glUniformMatrix4fv(location, N, GL_FALSE,
                       reinterpret_cast<const GLfloat*>(val.data()));
  }
};