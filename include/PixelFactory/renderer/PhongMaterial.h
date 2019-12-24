#pragma once

#include <memory>

class GLTexture2D;

struct PhongMaterial {
  std::shared_ptr<GLTexture2D> diffuse;
  std::shared_ptr<GLTexture2D> specular;
};


