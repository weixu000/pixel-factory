#include "PixelFactory/components/PointLight.h"

#include "PixelFactory/ModelLoader.h"
#include "PixelFactory/gl/GlBuffer.h"
#include "PixelFactory/gl/GlContext.h"
#include "PixelFactory/gl/GlTextureCubemap.h"
#include "PixelFactory/gl/GlVertexArray.h"

const float PointLight::z_near = 0.1f, PointLight::z_far = 100.0f;
const glm::mat4 PointLight::shadow_projection =
    glm::perspective(glm::half_pi<float>(), 1.0f, z_near, z_far);
const std::array<glm::mat4, 6> PointLight::shadow_view{
    glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)),
};

PointLight::PointLight(GlContext &context) : context_(context) {
  shadow_map =
      std::make_shared<GlTextureCubemap>(context_.CreateTextureCubemap());
  shadow_map->ImmutableStorage(1, GL_DEPTH_COMPONENT16, 1024);
  shadow_map->SetFilter(GL_NEAREST, GL_NEAREST);

  auto [indices, attributes] = LoadVerticesFromObjFile("meshes/sphere.obj");
  count = indices.size();
  auto vbo = context_.CreateBuffer();
  vbo.ImmutableStorage(sizeof(Vertex) * attributes.size(), attributes.data());
  auto ebo = context_.CreateBuffer();
  ebo.ImmutableStorage(sizeof(GLuint) * indices.size(), indices.data());
  vao = std::make_shared<GlVertexArray>(context_.CreateVertexArray());
  vao->BindVertexBuffer(0, vbo, 0, sizeof(Vertex));
  vao->SetAttribFormat<Vertex>(0);
  vao->BindElementBuffer(ebo);
}
