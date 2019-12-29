#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "PixelFactory/components/Component.h"

class GlContext;
class GlVertexArray;
class GlTextureCubemap;

class PointLight : public Component {
 public:
  glm::vec3 color;
  float attenuation;
  float fall_off;

  explicit PointLight(GlContext &context);

  std::shared_ptr<GlTextureCubemap> shadow_map;

  std::shared_ptr<GlVertexArray> vao;
  GLuint count;

  static const float z_near, z_far;
  static const glm::mat4 shadow_projection;
  static const std::array<glm::mat4, 6> shadow_view;

 private:
  GlContext &context_;
};
