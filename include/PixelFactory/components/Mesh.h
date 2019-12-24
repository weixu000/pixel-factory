#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/components/Component.h>
#include <PixelFactory/renderer/PhongMaterial.h>
#include <PixelFactory/ModelLoader.h>

class GlVertexArray;

class Mesh : public Component {
 public:
  Mesh(const std::vector<GLuint> &indices, const std::vector<VertexNormalTex> &attributes);

  static Mesh FromObjFile(const std::string &file_path);

  PhongMaterial material_;
  const GLsizei count_;
  std::shared_ptr<GlVertexArray> vao_;
};


