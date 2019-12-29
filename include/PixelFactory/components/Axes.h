#pragma once

#include <memory>

#include "PixelFactory/components/Component.h"

class GlContext;
class GlVertexArray;
class GlProgram;

class Axes : public Component {
 public:
  explicit Axes(GlContext &context);

  void Draw(const DrawOptions &options);

 private:
  GlContext &context_;
  std::shared_ptr<GlVertexArray> vao_;
  std::shared_ptr<GlProgram> shader_;
};
