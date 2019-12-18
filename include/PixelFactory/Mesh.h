#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/Component.h>
#include <PixelFactory/PhongMaterial.h>

class GLVertexArray;

class Mesh : public Component {
 public:
  using Attribute = std::tuple<glm::vec3, glm::vec3, glm::vec2>;
  Mesh(const std::vector<GLuint> &indices, const std::vector<Attribute> &attributes);

  static Mesh FromObjFile(const std::string &file_path);

  PhongMaterial material_;
  const GLsizei count_;
  std::shared_ptr<GLVertexArray> vao_;
};


