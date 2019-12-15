#pragma once

#include <glm/glm.hpp>

struct Transform {
  Transform(const glm::mat4 &m = glm::mat4(1.0f)) : matrix(m) {}

  operator glm::mat4() { return matrix; }

  glm::mat4 matrix;
};


