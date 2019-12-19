#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/Component.h>
#include <PixelFactory/PhongMaterial.h>
#include <PixelFactory/ModelLoader.h>

class GLVertexArray;

class Mesh : public Component {
 public:
  Mesh(const std::vector<GLuint> &indices, const std::vector<VertexNormalTex> &attributes);

  static Mesh FromObjFile(const std::string &file_path);

  PhongMaterial material_;
  const GLsizei count_;
  std::shared_ptr<GLVertexArray> vao_;
};


