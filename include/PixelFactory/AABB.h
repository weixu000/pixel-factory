#pragma once

#include <array>
#include <glm/glm.hpp>

struct AABB {
  AABB() = default;

  AABB(const glm::vec3 &min_val, const glm::vec3 &max_val);

  [[nodiscard]] glm::vec3 Min() const { return vertices[0]; }

  [[nodiscard]] glm::vec3 Max() const { return vertices[6]; }

  /*    y
   *    4----7
   *   /|   /|
   *  5-+--6 |
   *  | 0--+-3 x
   *  |/   |/
   *  1----2
   * z
   */
  std::array<glm::vec3, 8> vertices{};
};


