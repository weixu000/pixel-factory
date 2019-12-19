#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/Component.h>

class GLVertexArray;

struct PointLight : public Component {
  glm::vec3 color;
  float attenuation;
  float fall_off;

  PointLight();

  static inline std::unique_ptr<GLVertexArray> vao;
  static inline GLuint count;
};
