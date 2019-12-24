#pragma once

#include <memory>

#include <PixelFactory/components/Component.h>

class Axes : public Component {
 public:
  Axes();

  void Draw(const DrawOptions &options);
};


