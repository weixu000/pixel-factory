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
  struct Attribute {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
  };

  Mesh(const std::vector<GLuint> &indices, const std::vector<Attribute> &attributes);

  static Mesh FromObjFile(const std::string &file_path);

  PhongMaterial material_;
  const GLsizei count_;
  std::shared_ptr<GLVertexArray> vao_;
};


