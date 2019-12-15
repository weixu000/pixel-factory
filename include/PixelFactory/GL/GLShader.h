#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include <PixelFactory/GL/GLUniform.h>

class GLShader {
 public:
  GLShader() : id_(glCreateProgram()) {}

  GLShader(const std::string &vertex_file_path, const std::string &fragment_file_path,
           const std::string &geometry_file_path = "");

  ~GLShader() { glDeleteProgram(id_); }

  void Use() { glUseProgram(id_); }

  template<typename T>
  void SetUniform(const std::string &name, const T &val) {
    Uniform<T> uniform{name, val};

    auto iter = uniform_locations_.find(uniform.name);
    if (iter != uniform_locations_.end()) {
      uniform.Set(iter->second);
    }
  }

 private:
  GLuint id_ = 0;

  std::unordered_map<std::string, GLint> uniform_locations_;

  void CompileProgram(const std::string &vertex_file_path, const std::string &fragment_file_path,
                      const std::string &geometry_file_path);
};


