#include <PixelFactory/AABB.h>

AABB::AABB(const glm::vec3 &min_val, const glm::vec3 &max_val) {
  vertices[0] = vertices[1] = vertices[2] = vertices[3] = min_val;
  vertices[1].z = vertices[2].z = max_val.z;
  vertices[2].x = vertices[3].x = max_val.x;
  vertices[4] = vertices[0];
  vertices[5] = vertices[1];
  vertices[6] = vertices[2];
  vertices[7] = vertices[3];
  vertices[4].y = vertices[5].y = vertices[6].y = vertices[7].y = max_val.y;
}
