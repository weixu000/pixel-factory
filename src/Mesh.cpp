#include <iostream>
#include <map>
#include <numeric>
#include <tiny_obj_loader.h>

#include <PixelFactory/Mesh.h>
#include <PixelFactory/GLVertexArray.h>
#include <PixelFactory/GLBuffer.h>
#include <PixelFactory/GLShader.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/DrawOptions.h>
#include <PixelFactory/Camera.h>
#include <PixelFactory/AABB.h>

namespace {
std::unique_ptr<GLShader> shader_;
}

Mesh::Mesh(const std::vector<GLuint> &indices, const std::vector<glm::vec3> &attributes) {
  if (shader_ == nullptr) {
    shader_ = std::make_unique<GLShader>("shaders/flat.vert", "shaders/flat.frag");
  }

  count_ = indices.size();
  vao_ = std::make_unique<GLVertexArray>();

  vao_->Bind();
  GLBuffer vbo, ebo;

  vbo.Bind(GLBufferTarget::ArrayBuffer);
  vbo.Upload(sizeof(glm::vec3) * attributes.size(), attributes.data());
  vao_->SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         2 * sizeof(glm::vec3));
  vao_->SetAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                         2 * sizeof(glm::vec3), sizeof(glm::vec3));
  vbo.Unbind();

  ebo.Bind(GLBufferTarget::ElementArrayBuffer);
  // Pass in the data.
  ebo.Upload(sizeof(GLuint) * indices.size(), indices.data());

  // Unbind from the VAO.
  vao_->Unbind();
}

void Mesh::Draw(const DrawOptions &options) {
  shader_->Use();
  shader_->SetUniform("projection", options.camera.Projection());
  shader_->SetUniform("view", options.camera.View());
  shader_->SetUniform("world", entity_->WorldTransform().matrix);
  shader_->SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));

  vao_->Bind();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES, count_, GL_UNSIGNED_INT, nullptr);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  vao_->Unbind();
}

Mesh Mesh::FromObjFile(const std::string &file_path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;

  std::string warn;
  if (std::string err;!tinyobj::LoadObj(&attrib, &shapes, nullptr, &warn, &err, file_path.c_str())) {
    throw std::runtime_error("tinyobj::LoadObj error: " + err);
  }
  if (!warn.empty()) {
    std::cout << "tinyobj::LoadObj warning: " << warn << std::endl;
  }

  std::vector<glm::vec3> attributes; // vertices and normals interleaved
  std::vector<GLuint> indices;

  // Use std::map to record each different vertex/normal pairs
  std::map<std::tuple<GLuint, GLuint>, GLuint> indices_map;
  for (auto &shape:shapes) {
    for (auto it = shape.mesh.indices.begin(); it != shape.mesh.indices.end(); ++it) {
      auto v = 3 * it->vertex_index;
      auto n = 3 * it->normal_index;
      auto[it_index, inserted] = indices_map.insert(std::make_pair(std::make_tuple(v, n), attributes.size() >> 1U));
      if (inserted) {
        attributes.emplace_back(attrib.vertices[v], attrib.vertices[v + 1], attrib.vertices[v + 2]);
        attributes.emplace_back(attrib.normals[n], attrib.normals[n + 1], attrib.normals[n + 2]);
      }
      indices.push_back(it_index->second);
    }
  }

  return Mesh(indices, attributes);
}

Mesh Mesh::FaceNormalMesh(const std::vector<GLuint> &indices, const std::vector<glm::vec3> &vertices) {
  std::vector<glm::vec3> attributes;
  for (size_t i = 0; i < indices.size(); i += 3) {
    std::array<glm::vec3, 3> p = {vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]};
    auto normal = glm::cross(p[1] - p[0], p[2] - p[0]);
    for (auto &v:p) {
      attributes.push_back(v);
      attributes.push_back(normal);
    }
  }

  std::vector<GLuint> new_indices(attributes.size() / 2);
  std::iota(new_indices.begin(), new_indices.end(), 0);

  return Mesh(new_indices, attributes);
}

Mesh Mesh::FromAABB(const AABB &bb) {
  std::vector<glm::vec3> vertices(bb.vertices.begin(), bb.vertices.end());
  std::vector<GLuint> indices{
      0, 2, 1, 0, 3, 2,
      0, 1, 4, 1, 5, 4,
      0, 7, 3, 0, 4, 7,
      0, 2, 1, 0, 3, 2,
      1, 2, 5, 2, 6, 5,
      2, 3, 6, 3, 7, 6,
      4, 5, 6, 4, 6, 7
  };
  return FaceNormalMesh(indices, vertices);
}

Mesh Mesh::Cube() {
  return FromAABB(AABB(glm::vec3(-0.5f), glm::vec3(0.5f)));
}
