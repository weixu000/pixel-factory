#pragma once

#include <glm/glm.hpp>

#include <PixelFactory/Component.h>

struct PointLight : public Component {
  glm::vec3 color;
  float attenuation;
};


