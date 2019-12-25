#include <glm/gtx/transform.hpp>

#include <PixelFactory/renderer/DeferredRenderer.h>
#include <PixelFactory/gl/GlFramebuffer.h>
#include <PixelFactory/gl/GlVertexArray.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/gl/GlShader.h>
#include <PixelFactory/renderer/DrawOptions.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/components/PointLight.h>

namespace {
std::unique_ptr<GlShader> geomtery_pass_shader;
std::unique_ptr<GlShader> lighting_pass_shader;
std::unique_ptr<GlShader> shadow_pass_shader;
}

DeferredRenderer::DeferredRenderer(int width, int height)
    : width_(width), height_(height) {
  if (geomtery_pass_shader == nullptr) {
    geomtery_pass_shader = std::make_unique<GlShader>("shaders/g_buffer.vert", "shaders/g_buffer.frag");
    lighting_pass_shader = std::make_unique<GlShader>("shaders/flat.vert", "shaders/pointlight_phong.frag");
    shadow_pass_shader =
        std::make_unique<GlShader>("shaders/shadow_map.vert", "shaders/shadow_map.frag", "shaders/shadow_map.geom");
  }

  gbuffer_ = std::make_unique<GlFramebuffer>();

  position_ = std::make_unique<GlTexture2D>(1, GL_RGB16F, width, height);
  position_->SetFilter(GL_NEAREST, GL_NEAREST);
  gbuffer_->Attach(GL_COLOR_ATTACHMENT0, *position_);

  normal_ = std::make_unique<GlTexture2D>(1, GL_RGB16F, width, height);
  normal_->SetFilter(GL_NEAREST, GL_NEAREST);
  gbuffer_->Attach(GL_COLOR_ATTACHMENT1, *normal_);

  albedo_spec_ = std::make_unique<GlTexture2D>(1, GL_RGBA8, width, height);
  albedo_spec_->SetFilter(GL_NEAREST, GL_NEAREST);
  gbuffer_->Attach(GL_COLOR_ATTACHMENT2, *albedo_spec_);

  gbuffer_->DrawBuffers({GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2});

  depth_ = std::make_unique<GlRenderbuffer>(GL_DEPTH_COMPONENT24, width, height);
  gbuffer_->Attach(GL_DEPTH_ATTACHMENT, *depth_);

  if (!gbuffer_->IsComplete()) {
    std::runtime_error("GBuffer not complete");
  }

  shadow_ = std::make_unique<GlFramebuffer>();
  shadow_->ReadBuffer(GL_NONE);
  shadow_->DrawBuffers({GL_NONE});
}

void DeferredRenderer::Collect(const Entity &scene) {
  meshes_.clear();
  lights_.clear();
  scene.ForEachDescendant([this](const Entity &child) {
    child.ForEachComponent([this](const Component &component) {
      if (auto p_mesh = dynamic_cast<const Mesh *>(&component)) {
        meshes_.push_back(p_mesh);
      } else if (auto p_light = dynamic_cast<const PointLight *>(&component)) {
        lights_.push_back(p_light);
      }
    });
  });
}

void DeferredRenderer::GeometryPass(const DrawOptions &options) {
  gbuffer_->Bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  geomtery_pass_shader->Use();
  geomtery_pass_shader->SetUniform("projection", options.camera.Projection());
  geomtery_pass_shader->SetUniform("view", options.camera.View());
  for (auto &mesh:meshes_) {
    geomtery_pass_shader->SetUniform("world", mesh->GetEntity()->WorldTransform().matrix);

    glActiveTexture(GL_TEXTURE0);
    mesh->material_.diffuse->Bind();
    geomtery_pass_shader->SetUniform("diffuseTex", 0);

    glActiveTexture(GL_TEXTURE1);
    mesh->material_.specular->Bind();
    geomtery_pass_shader->SetUniform("specularTex", 1);

    mesh->vao_->Bind();
    glDrawElements(GL_TRIANGLES, mesh->count_, GL_UNSIGNED_INT, nullptr);
    mesh->vao_->Unbind();
  }
  gbuffer_->Unbind();
}

void DeferredRenderer::LightingPass(const DrawOptions &options) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  lighting_pass_shader->Use();
  glActiveTexture(GL_TEXTURE0);
  position_->Bind();
  lighting_pass_shader->SetUniform("gPosition", 0);
  glActiveTexture(GL_TEXTURE1);
  normal_->Bind();
  lighting_pass_shader->SetUniform("gNormal", 1);
  glActiveTexture(GL_TEXTURE2);
  albedo_spec_->Bind();
  lighting_pass_shader->SetUniform("gAlbedoSpec", 2);
  lighting_pass_shader->SetUniform("eye", options.camera.Eye());

  lighting_pass_shader->SetUniform("projection", options.camera.Projection());
  lighting_pass_shader->SetUniform("view", options.camera.View());

  // Add effect of each light
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDisable(GL_DEPTH_TEST); // Render every light
  // Render back faces of light volume so light is working when camera inside
  glCullFace(GL_FRONT);
  for (auto &light:lights_) {
    lighting_pass_shader->SetUniform("light.position", light->GetEntity()->WorldTransform().Translation());
    lighting_pass_shader->SetUniform("light.color", light->color);
    lighting_pass_shader->SetUniform("light.attenuation", light->attenuation);
    lighting_pass_shader->SetUniform("light.fallOff", light->fall_off);

    lighting_pass_shader->SetUniform("world",
                                     light->GetEntity()->WorldTransform().matrix
                                         * glm::scale(glm::vec3(light->fall_off)));

    glActiveTexture(GL_TEXTURE3);
    light->shadow_map->Bind();
    lighting_pass_shader->SetUniform("shadowMap", 3);

    PointLight::vao->Bind();
    glDrawElements(GL_TRIANGLES, PointLight::count, GL_UNSIGNED_INT, nullptr);
    PointLight::vao->Unbind();
  }
  // Reset the states
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  // Copy depth buffer to default framebuffer for future use
  glBlitNamedFramebuffer(gbuffer_->Id(), 0,
                         0, 0, width_, height_,
                         0, 0, width_, width_,
                         GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void DeferredRenderer::ShadowPass() {
  shadow_->Bind();
  shadow_pass_shader->Use();

  for (auto &light:lights_) {
    shadow_->Attach(GL_DEPTH_ATTACHMENT, *light->shadow_map);

    if (!shadow_->IsComplete()) {
      throw std::runtime_error("Shadow FBO not complete");
    }

    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, light->shadow_map->size_, light->shadow_map->size_);

    auto light_translation = glm::translate(-light->GetEntity()->WorldTransform().Translation());
    std::array<glm::mat4, 6> light_views{};
    for (int face = 0; face < 6; ++face) {
      light_views[face] = PointLight::shadow_view[face] * light_translation;
    }
    shadow_pass_shader->SetUniform("lightView[0]", light_views);
    shadow_pass_shader->SetUniform("lightProj", PointLight::shadow_projection);
    shadow_pass_shader->SetUniform("fallOff", light->fall_off);

    for (auto &mesh:meshes_) {
      shadow_pass_shader->SetUniform("world", mesh->GetEntity()->WorldTransform().matrix);

      mesh->vao_->Bind();
      glDrawElements(GL_TRIANGLES, mesh->count_, GL_UNSIGNED_INT, nullptr);
      mesh->vao_->Unbind();
    }
  }

  shadow_->Unbind();

  glViewport(0, 0, width_, height_);
}

void DeferredRenderer::DrawGBuffer(GLenum mode) {
  gbuffer_->ReadBuffer(mode);

  glBlitNamedFramebuffer(gbuffer_->Id(), 0,
                         0, 0, width_, height_,
                         0, 0, width_, width_,
                         GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
