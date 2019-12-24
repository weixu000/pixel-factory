#pragma once

#include <memory>
#include <list>
#include <glad/glad.h>

class GlTexture2D;
class GlRenderbuffer;
class GlFramebuffer;
class Entity;
class Mesh;
class PointLight;
class DrawOptions;

class DeferredRenderer {
 public:
  DeferredRenderer(int width, int height);

  void Collect(const Entity &scene);

  void GeometryPass(const DrawOptions &options);

  void LightingPass(const DrawOptions &options);

  void DrawPositonMap() { DrawGBuffer(GL_COLOR_ATTACHMENT0); }

  void DrawNormalMap() { DrawGBuffer(GL_COLOR_ATTACHMENT1); }

  void DrawAlbedoSpecMap() { DrawGBuffer(GL_COLOR_ATTACHMENT2); }

 private:
  std::unique_ptr<GlTexture2D> position_, normal_, albedo_spec_;
  std::unique_ptr<GlRenderbuffer> depth_;
  std::unique_ptr<GlFramebuffer> gbuffer_;

  std::list<const Mesh *> meshes_;
  std::list<const PointLight *> lights_;

  const int width_, height_;

  void DrawGBuffer(GLenum mode);
};
