#pragma once

#include <memory>

#include "PixelFactory/components/Component.h"

class GlContext;

class Axes : public Component {
 public:
  explicit Axes(GlContext &context);

  void Draw(const DrawOptions &options);

 private:
  GlContext &context_;
};
