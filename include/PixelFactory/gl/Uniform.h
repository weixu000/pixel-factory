#pragma once

#include <string>
#include <memory>
#include <list>
#include <utility>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
struct Uniform {
  std::string name;
  T val;
  void Set(GLint location) const;
};

using Uniform1i = Uniform<int>;
using Uniform1f = Uniform<float>;
using UniformVec3 = Uniform<glm::vec3>;
using UniformMat4 = Uniform<glm::mat4>;

template<>
inline void Uniform<int>::Set(GLint location) const {
  glUniform1i(location, val);
}

template<>
inline void Uniform<float>::Set(GLint location) const {
  glUniform1f(location, val);
}

template<>
inline void Uniform<glm::vec3>::Set(GLint location) const {
  glUniform3fv(location, 1, glm::value_ptr(val));
}

template<>
inline void Uniform<glm::mat4>::Set(GLint location) const {
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
}