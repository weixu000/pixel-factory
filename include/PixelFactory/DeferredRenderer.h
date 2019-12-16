#pragma once

#include <memory>
#include <list>

class GLTexture2D;
class GLRenderbuffer;
class GLFramebuffer;
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

  std::unique_ptr<GLTexture2D> position_, normal_, albedo_spec_;
  std::unique_ptr<GLRenderbuffer> depth_;
  std::unique_ptr<GLFramebuffer> gbuffer_;

  std::list<const Mesh *> meshes_;
  std::list<const PointLight *> lights_;

  const int width_, height_;
};
