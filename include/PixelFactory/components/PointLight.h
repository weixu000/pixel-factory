#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PixelFactory/components/Component.h>

class GlVertexArray;
class GlTextureCubemap;

struct PointLight : public Component {
  glm::vec3 color;
  float attenuation;
  float fall_off;

  PointLight();

  std::unique_ptr<GlTextureCubemap> shadow_map;

  static inline const float z_near = 0.1f, z_far = 100.0f;

  static inline const glm::mat4 shadow_projection = glm::perspective(glm::half_pi<float>(),
                                                                     1.0f,
                                                                     z_near, z_far);

  static inline const std::array<glm::mat4, 6> shadow_view{
      glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
  };

  static inline std::unique_ptr<GlVertexArray> vao;
  static inline GLuint count;
};
