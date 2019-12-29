#include "PixelFactory/renderer/DeferredRenderer.h"

#include <spdlog/spdlog.h>

#include <glm/gtx/transform.hpp>

#include "PixelFactory/Entity.h"
#include "PixelFactory/components/Camera.h"
#include "PixelFactory/components/Mesh.h"
#include "PixelFactory/components/PointLight.h"
#include "PixelFactory/gl/GlContext.h"
#include "PixelFactory/gl/GlProgram.h"
#include "PixelFactory/renderer/DrawOptions.h"

DeferredRenderer::DeferredRenderer(GlContext &context, int width, int height)
    : context_(context), width_(width), height_(height) {
  geomtery_shader_ = std::make_unique<GlProgram>(
      context_.CreateProgram("shaders/g_buffer.vert", "shaders/g_buffer.frag"));
  lighting_shader_ = std::make_unique<GlProgram>(context_.CreateProgram(
      "shaders/flat.vert", "shaders/pointlight_phong.frag"));
  shadow_shader_ = std::make_unique<GlProgram>(context_.CreateProgram(
      "shaders/shadow_map.vert", "shaders/shadow_map.frag",
      "shaders/shadow_map.geom"));
  shadow_shader_ = std::make_unique<GlProgram>(context_.CreateProgram(
      "shaders/shadow_map.vert", "shaders/shadow_map.frag",
      "shaders/shadow_map.geom"));

  gbuffer_ = std::make_unique<GlFramebuffer>(context_.CreateFramebuffer());

  position_ = std::make_unique<GlTexture2D>(context_.CreateTexture2D());
  position_->ImmutableStorage(1, GL_RGB16F, width_, height_);
  position_->SetFilter(GL_NEAREST, GL_NEAREST);
  gbuffer_->Attach(GL_COLOR_ATTACHMENT0, *position_);

  normal_ = std::make_unique<GlTexture2D>(context_.CreateTexture2D());
  normal_->ImmutableStorage(1, GL_RGB16F, width_, height_);
  normal_->SetFilter(GL_NEAREST, GL_NEAREST);
  gbuffer_->Attach(GL_COLOR_ATTACHMENT1, *normal_);

  albedo_spec_ = std::make_unique<GlTexture2D>(context_.CreateTexture2D());
  albedo_spec_->ImmutableStorage(1, GL_RGBA8, width_, height_);
  albedo_spec_->SetFilter(GL_NEAREST, GL_NEAREST);
  gbuffer_->Attach(GL_COLOR_ATTACHMENT2, *albedo_spec_);

  gbuffer_->DrawBuffers(
      {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2});

  depth_ = std::make_unique<GlRenderbuffer>(context_.CreateRenderbuffer());
  depth_->ImmutableStorage(GL_DEPTH_COMPONENT24, width_, height);
  gbuffer_->Attach(GL_DEPTH_ATTACHMENT, *depth_);

  if (!gbuffer_->IsComplete()) {
    std::runtime_error("GBuffer not complete");
  }

  shadow_ = std::make_unique<GlFramebuffer>(context_.CreateFramebuffer());
  shadow_->ReadBuffer(GL_NONE);
  shadow_->DrawBuffers({GL_NONE});

  spdlog::debug("DeferredRenderer Created");
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
  spdlog::debug("DeferredRenderer Collected {} meshes, {} lights",
                meshes_.size(), lights_.size());
}

void DeferredRenderer::GeometryPass(const DrawOptions &options) {
  spdlog::debug("DeferredRenderer geometry pass");
  context_.Bind(FramebufferTarget::Framebuffer, *gbuffer_);
  context_.Clear(true, true);

  context_.Use(*geomtery_shader_);
  geomtery_shader_->SetUniform("projection", options.camera.Projection());
  geomtery_shader_->SetUniform("view", options.camera.View());
  for (auto &mesh : meshes_) {
    geomtery_shader_->SetUniform("world",
                                 mesh->GetEntity()->WorldTransform().matrix);

    glActiveTexture(GL_TEXTURE0);
    context_.Bind(*mesh->material_.diffuse);
    geomtery_shader_->SetUniform("diffuseTex", 0);

    glActiveTexture(GL_TEXTURE1);
    context_.Bind(*mesh->material_.specular);
    geomtery_shader_->SetUniform("specularTex", 1);

    context_.Bind(*mesh->vao_);
    glDrawElements(GL_TRIANGLES, mesh->count_, GL_UNSIGNED_INT, nullptr);
  }
  context_.Bind(FramebufferTarget::Framebuffer, context_.DefaultFramebuffer());
}

void DeferredRenderer::LightingPass(const DrawOptions &options) {
  spdlog::debug("DeferredRenderer lighting pass");
  context_.Clear(true, true);

  context_.Use(*lighting_shader_);
  glActiveTexture(GL_TEXTURE0);
  context_.Bind(*position_);
  lighting_shader_->SetUniform("gPosition", 0);
  glActiveTexture(GL_TEXTURE1);
  context_.Bind(*normal_);
  lighting_shader_->SetUniform("gNormal", 1);
  glActiveTexture(GL_TEXTURE2);
  context_.Bind(*albedo_spec_);
  lighting_shader_->SetUniform("gAlbedoSpec", 2);
  lighting_shader_->SetUniform("eye", options.camera.Eye());

  lighting_shader_->SetUniform("projection", options.camera.Projection());
  lighting_shader_->SetUniform("view", options.camera.View());

  // Add effect of each light
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDisable(GL_DEPTH_TEST);  // Render every light
  // Render back faces of light volume so light is working when camera inside
  glCullFace(GL_FRONT);
  for (auto &light : lights_) {
    lighting_shader_->SetUniform(
        "light.position", light->GetEntity()->WorldTransform().Translation());
    lighting_shader_->SetUniform("light.color", light->color);
    lighting_shader_->SetUniform("light.attenuation", light->attenuation);
    lighting_shader_->SetUniform("light.fallOff", light->fall_off);

    lighting_shader_->SetUniform("world",
                                 light->GetEntity()->WorldTransform().matrix *
                                     glm::scale(glm::vec3(light->fall_off)));

    glActiveTexture(GL_TEXTURE3);
    context_.Bind(*light->shadow_map);
    lighting_shader_->SetUniform("shadowMap", 3);

    context_.Bind(*light->vao);
    glDrawElements(GL_TRIANGLES, light->count, GL_UNSIGNED_INT, nullptr);
  }
  // Reset the states
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  // Copy depth buffer to default framebuffer for future use
  glBlitNamedFramebuffer(gbuffer_->Id(), context_.DefaultFramebuffer().Id(), 0,
                         0, width_, height_, 0, 0, width_, height_,
                         GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void DeferredRenderer::ShadowPass() {
  spdlog::debug("DeferredRenderer shadow pass");
  context_.Bind(FramebufferTarget::Framebuffer, *shadow_);
  context_.Use(*shadow_shader_);

  for (auto &light : lights_) {
    shadow_->Attach(GL_DEPTH_ATTACHMENT, *light->shadow_map);
    if (!shadow_->IsComplete()) {
      throw std::runtime_error("Shadow FBO not complete");
    }

    context_.Clear(false, true);
    context_.Viewport(0, 0, light->shadow_map->Size(),
                      light->shadow_map->Size());

    auto light_translation =
        glm::translate(-light->GetEntity()->WorldTransform().Translation());
    std::array<glm::mat4, 6> light_views{};
    for (int face = 0; face < 6; ++face) {
      light_views[face] = PointLight::shadow_view[face] * light_translation;
    }
    shadow_shader_->SetUniform("lightView[0]", light_views);
    shadow_shader_->SetUniform("lightProj", PointLight::shadow_projection);
    shadow_shader_->SetUniform("fallOff", light->fall_off);

    for (auto &mesh : meshes_) {
      shadow_shader_->SetUniform("world",
                                 mesh->GetEntity()->WorldTransform().matrix);
      context_.Bind(*mesh->vao_);
      glDrawElements(GL_TRIANGLES, mesh->count_, GL_UNSIGNED_INT, nullptr);
    }
  }

  context_.Bind(FramebufferTarget::Framebuffer, context_.DefaultFramebuffer());
  context_.Viewport(0, 0, width_, height_);
}

void DeferredRenderer::DrawGBuffer(GLenum mode) {
  gbuffer_->ReadBuffer(mode);
  glBlitNamedFramebuffer(gbuffer_->Id(), context_.DefaultFramebuffer().Id(), 0,
                         0, width_, height_, 0, 0, width_, height_,
                         GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
