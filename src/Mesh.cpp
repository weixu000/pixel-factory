#include <iostream>
#include <map>
#include <tiny_obj_loader.h>

#include <PixelFactory/Mesh.h>
#include <PixelFactory/GL/GLVertexArray.h>
#include <PixelFactory/GL/GLBuffer.h>
#include <PixelFactory/Entity.h>

Mesh::Mesh(const std::vector<GLuint> &indices, const std::vector<Attribute> &attributes)
    : count_(indices.size()), vao_(std::make_shared<GLVertexArray>()) {
  vao_->Bind();
  GLBuffer vbo, ebo;

  vbo.Bind(GLBuffer::Target::ArrayBuffer);
  vbo.Upload(sizeof(Attribute) * attributes.size(), attributes.data());
  vao_->SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(Attribute));
  vao_->SetAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                         sizeof(Attribute), sizeof(glm::vec3));
  vao_->SetAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                         sizeof(Attribute), 2 * sizeof(glm::vec3));
  vbo.Unbind();

  ebo.Bind(GLBuffer::Target::ElementArrayBuffer);
  // Pass in the data.
  ebo.Upload(sizeof(GLuint) * indices.size(), indices.data());

  // Unbind from the VAO.
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

  std::vector<Attribute> attributes; // vertices and normals interleaved
  std::vector<GLuint> indices;

  // Use std::map to record each different vertex/normal pairs
  std::map<std::tuple<GLuint, GLuint, GLuint>, GLuint> indices_map;
  for (auto &shape:shapes) {
    for (auto &index : shape.mesh.indices) {
      auto v = 3 * index.vertex_index;
      auto n = 3 * index.normal_index;
      auto t = 2 * index.texcoord_index;
      auto[it_index, inserted] = indices_map.insert(std::make_pair(std::make_tuple(v, n, t), attributes.size()));
      if (inserted) {
        attributes.push_back(Attribute{
            glm::vec3(attrib.vertices[v], attrib.vertices[v + 1], attrib.vertices[v + 2]),
            glm::vec3(attrib.normals[n], attrib.normals[n + 1], attrib.normals[n + 2]),
            glm::vec2(attrib.texcoords[t], attrib.texcoords[t + 1])
        });
      }
      indices.push_back(it_index->second);
    }
  }
  return Mesh(indices, attributes);
}
