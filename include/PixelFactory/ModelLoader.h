#pragma once

#include <vector>
#include <tuple>
#include <glad/glad.h>
#include <glm/glm.hpp>

using Vertex = std::tuple<glm::vec3>;
using VertexNormalTex = std::tuple<glm::vec3, glm::vec3, glm::vec2>;

std::tuple<std::vector<GLuint>, std::vector<Vertex>> LoadVerticesFromObjFile(const std::string &file_path);

std::tuple<std::vector<GLuint>, std::vector<VertexNormalTex>> LoadObjFile(const std::string &file_path);

