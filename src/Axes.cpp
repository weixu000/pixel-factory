#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <PixelFactory/components/Axes.h>
#include <PixelFactory/gl/GlContext.h>
#include <PixelFactory/renderer/DrawOptions.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/gl/GlShader.h>
#include <PixelFactory/Entity.h>

namespace {
std::unique_ptr<GlVertexArray> vao_;
std::unique_ptr<GlShader> shader_;
}

Axes::Axes(GlContext &context) : context_(context) {
  if (vao_ == nullptr) {
    std::array<glm::vec3, 4> vertices = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    };

    auto vbo = context_.CreateBuffer();
    vbo.ImmutableStorage(sizeof(glm::vec3) * vertices.size(), vertices.data());
    vao_ = std::make_unique<GlVertexArray>(context_.CreateVertexArray());
    vao_->BindVertexBuffer(0, vbo, 0, sizeof(glm::vec3));
    vao_->SetAttribFormat<glm::vec3>(0, 0);
    vao_->AssociateAttrib(0, 0);

    shader_ = std::make_unique<GlShader>("shaders/flat.vert", "shaders/flat.frag");
  }
}

void Axes::Draw(const DrawOptions &options) {
  shader_->Use();
  shader_->SetUniform("projection", options.camera.Projection());
  shader_->SetUniform("view", options.camera.View());

  context_.Bind(*vao_);

  shader_->SetUniform("world", entity_->WorldTransform().matrix);
  shader_->SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
  glDrawArrays(GL_LINES, 0, 2);

  shader_->SetUniform("world", entity_->WorldTransform().matrix * glm::eulerAngleZ(glm::pi<float>() / 2));
  shader_->SetUniform("color", glm::vec3(0.0f, 1.0f, 0.0f));
  glDrawArrays(GL_LINES, 0, 2);

  shader_->SetUniform("world", entity_->WorldTransform().matrix * glm::eulerAngleY(-glm::pi<float>() / 2));
  shader_->SetUniform("color", glm::vec3(0.0f, 0.0f, 1.0f));
  glDrawArrays(GL_LINES, 0, 2);
}

