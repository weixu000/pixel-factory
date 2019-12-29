#pragma once

#include <glad/glad.h>

#include <cassert>
#include <string>
#include <unordered_map>

#include "PixelFactory/gl/GlShader.h"
#include "PixelFactory/gl/GlUniform.h"

class GlContext;

class GlProgram {
 public:
  GlProgram(GlProgram &&other) noexcept
      : id_(other.id_),
        uniform_locations_(std::move(other.uniform_locations_)) {
    other.id_ = 0U;
  }

  ~GlProgram() { glDeleteProgram(id_); }

  GlProgram &operator=(GlProgram &&other) noexcept {
    assert(this != &other);
    glDeleteProgram(id_);
    id_ = other.id_;
    other.id_ = 0U;
    uniform_locations_ = std::move(other.uniform_locations_);
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void Attach(GlShader &shader);

  void Link();

  void Detach(GlShader &shader);

  void GetActiveUniformLocations();

  template <typename T>
  void SetUniform(const std::string &name, const T &val) {
    GlUniform<T> uniform{val};
    auto iter = uniform_locations_.find(name);
    if (iter != uniform_locations_.end()) {
      uniform.Set(id_, iter->second);
    }
  }

 private:
  GLuint id_ = 0;

  friend class GlContext;
  explicit GlProgram(GLuint id) : id_(id) {}

  std::unordered_map<std::string, GLint> uniform_locations_;
};
