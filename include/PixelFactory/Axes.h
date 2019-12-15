#pragma once

#include <memory>

#include <PixelFactory/Component.h>

class Axes : public Component {
 public:
  Axes();

  void Draw(const DrawOptions &options) override;
};


