#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <PixelFactory/components/Axes.h>
#include <PixelFactory/GL/GLBuffer.h>
#include <PixelFactory/GL/GLVertexArray.h>
#include <PixelFactory/renderer/DrawOptions.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/GL/GLShader.h>
#include <PixelFactory/Entity.h>

namespace {
std::unique_ptr<GLVertexArray> vao_;
std::unique_ptr<GLShader> shader_;
}

Axes::Axes() {
  if (vao_ == nullptr) {
    std::array<glm::vec3, 4> vertices = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
    };

    vao_ = std::make_unique<GLVertexArray>();
    vao_->Bind();

    GLBuffer vbo;
    vbo.Bind(GLBuffer::Target::ArrayBuffer);
    vbo.Upload(sizeof(glm::vec3) * vertices.size(), vertices.data());

    vao_->SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3));

    vbo.Unbind();
    vao_->Unbind();

    shader_ = std::make_unique<GLShader>("shaders/flat.vert", "shaders/flat.frag");
  }
}

void Axes::Draw(const DrawOptions &options) {
  shader_->Use();
  shader_->SetUniform("projection", options.camera.Projection());
  shader_->SetUniform("view", options.camera.View());

  vao_->Bind();

  shader_->SetUniform("world", entity_->WorldTransform().matrix);
  shader_->SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
  glDrawArrays(GL_LINES, 0, 2);

  shader_->SetUniform("world", entity_->WorldTransform().matrix * glm::eulerAngleZ(glm::pi<float>() / 2));
  shader_->SetUniform("color", glm::vec3(0.0f, 1.0f, 0.0f));
  glDrawArrays(GL_LINES, 0, 2);

  shader_->SetUniform("world", entity_->WorldTransform().matrix * glm::eulerAngleY(-glm::pi<float>() / 2));
  shader_->SetUniform("color", glm::vec3(0.0f, 0.0f, 1.0f));
  glDrawArrays(GL_LINES, 0, 2);

  vao_->Unbind();
}

