#pragma once

#include <glm/glm.hpp>

struct Transform {
  Transform(const glm::mat4 &m = glm::mat4(1.0f)) : matrix(m) {}

  operator glm::mat4() { return matrix; }

  [[nodiscard]] glm::vec3 Translation() const { return glm::vec3(matrix[3]); }

  glm::mat4 matrix;
};


