#pragma once

#include <glad/glad.h>

#include <cassert>
#include <string>

enum class ShaderType : GLenum {
  VertexShader = GL_VERTEX_SHADER,
  GeometryShader = GL_GEOMETRY_SHADER,
  FragmentShader = GL_FRAGMENT_SHADER
};

class GlShader {
 public:
  GlShader() : id_(0U) {}

  explicit GlShader(ShaderType type)
      : id_(glCreateShader(static_cast<GLenum>(type))) {}

  GlShader(ShaderType type, const std::string &file_path) : GlShader(type) {
    Compile(file_path);
  }

  GlShader(GlShader &&other) noexcept : id_(other.id_) { other.id_ = 0U; }

  ~GlShader() { glDeleteShader(id_); }

  GlShader &operator=(GlShader &&other) noexcept {
    assert(this != &other);
    glDeleteShader(id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void Compile(const std::string &file_path);

 private:
  GLuint id_ = 0;
};
