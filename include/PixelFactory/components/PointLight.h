#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/components/Component.h>

class GlVertexArray;

struct PointLight : public Component {
  glm::vec3 color;
  float attenuation;
  float fall_off;

  PointLight();

  static inline std::unique_ptr<GlVertexArray> vao;
  static inline GLuint count;
};
