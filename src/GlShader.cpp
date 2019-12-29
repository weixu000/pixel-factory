#include "PixelFactory/gl/GlShader.h"

#include <spdlog/spdlog.h>

#include <fstream>

void GlShader::Compile(const std::string &file_path) {
  // Read shader code from the shader file.
  std::string code;
  std::ifstream stream(file_path, std::ios::in);
  if (stream.is_open()) {
    std::string line;
    while (getline(stream, line)) code += "\n" + line;
    stream.close();
  } else {
    throw std::runtime_error("Cannot open " + file_path);
  }

  // Compile Shader.
  char const *source = code.c_str();
  glShaderSource(id_, 1, &source, nullptr);
  glCompileShader(id_);

  // Check Shader.
  GLint result = GL_FALSE;
  glGetShaderiv(id_, GL_COMPILE_STATUS, &result);
  int log_length;
  glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0) {
    std::string msg(log_length, '\0');
    glGetShaderInfoLog(id_, log_length, nullptr, msg.data());
    spdlog::warn("Shader compilation log: {}", msg);
  }

  if (result != GL_TRUE) {
    throw std::runtime_error("Shader compilation error: " + file_path);
  }
  spdlog::debug("{} compiled", file_path);
}
