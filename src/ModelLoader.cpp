#include "PixelFactory/ModelLoader.h"

#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include <map>

namespace {
std::tuple<tinyobj::attrib_t, std::vector<tinyobj::shape_t>> LoadTinyObj(
    const std::string &file_path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;

  std::string warn;
  if (std::string err; !tinyobj::LoadObj(&attrib, &shapes, nullptr, &warn, &err,
                                         file_path.c_str())) {
    throw std::runtime_error("tinyobj::LoadObj error: " + err);
  }
  if (!warn.empty()) {
    spdlog::warn("tinyobj warning: {}", warn);
  }

  spdlog::debug("{} loaded", file_path);
  return std::make_tuple(attrib, shapes);
}
}  // namespace

std::tuple<std::vector<GLuint>, std::vector<VertexNormalTex>> LoadObjFile(
    const std::string &file_path) {
  auto [attrib, shapes] = LoadTinyObj(file_path);

  std::vector<VertexNormalTex> attributes;
  std::vector<GLuint> indices;

  // Use std::map to deduplicate
  std::map<std::tuple<GLuint, GLuint, GLuint>, GLuint> indices_map;
  for (auto &shape : shapes) {
    for (auto &index : shape.mesh.indices) {
      auto v = 3 * index.vertex_index;
      auto n = 3 * index.normal_index;
      auto t = 2 * index.texcoord_index;
      auto [it_index, inserted] = indices_map.insert(
          std::make_pair(std::make_tuple(v, n, t), attributes.size()));
      if (inserted) {
        attributes.emplace_back(
            glm::vec3(attrib.vertices[v], attrib.vertices[v + 1],
                      attrib.vertices[v + 2]),
            glm::vec3(attrib.normals[n], attrib.normals[n + 1],
                      attrib.normals[n + 2]),
            glm::vec2(attrib.texcoords[t], attrib.texcoords[t + 1]));
      }
      indices.push_back(it_index->second);
    }
  }
  return std::make_tuple(indices, attributes);
}

std::tuple<std::vector<GLuint>, std::vector<Vertex>> LoadVerticesFromObjFile(
    const std::string &file_path) {
  auto [attrib, shapes] = LoadTinyObj(file_path);

  std::vector<Vertex> attributes;
  std::vector<GLuint> indices;

  std::map<GLuint, GLuint> indices_map;
  for (auto &shape : shapes) {
    for (auto &index : shape.mesh.indices) {
      auto v = 3 * index.vertex_index;
      auto [it_index, inserted] =
          indices_map.insert(std::make_pair(v, attributes.size()));
      if (inserted) {
        attributes.emplace_back(glm::vec3(attrib.vertices[v],
                                          attrib.vertices[v + 1],
                                          attrib.vertices[v + 2]));
      }
      indices.push_back(it_index->second);
    }
  }
  return std::make_tuple(indices, attributes);
}
