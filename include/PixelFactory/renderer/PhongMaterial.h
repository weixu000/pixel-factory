#pragma once

#include <memory>

class GlTexture2D;

struct PhongMaterial {
  std::shared_ptr<GlTexture2D> diffuse;
  std::shared_ptr<GlTexture2D> specular;
};


