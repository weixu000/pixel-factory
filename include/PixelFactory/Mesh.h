#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <PixelFactory/Component.h>

class GLVertexArray;
class AABB;

class Mesh : public Component {
 public:
  Mesh(const std::vector<GLuint> &indices, const std::vector<glm::vec3> &attributes);

  void Draw(const DrawOptions &options) override;

  static Mesh FromObjFile(const std::string &file_path);

  static Mesh FaceNormalMesh(const std::vector<GLuint> &indices, const std::vector<glm::vec3> &vertices);

  static Mesh FromAABB(const AABB &bb);

  static Mesh Cube();

 private:
  GLsizei count_ = 0;
  std::unique_ptr<GLVertexArray> vao_;
};


