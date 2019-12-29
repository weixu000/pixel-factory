#include "PixelFactory/gl/GlProgram.h"

#include <spdlog/spdlog.h>

void GlProgram::Attach(GlShader &shader) { glAttachShader(id_, shader.Id()); }

void GlProgram::Link() {
  glLinkProgram(id_);

  // Check the program.
  GLint result = GL_FALSE;
  glGetProgramiv(id_, GL_LINK_STATUS, &result);
  int log_length;
  glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0) {
    std::string msg(log_length, '\0');
    glGetProgramInfoLog(id_, log_length, nullptr, msg.data());
    glDeleteProgram(id_);
    spdlog::warn("Program linking log: {}", msg);
  }

  if (result != GL_TRUE) {
    throw std::runtime_error("Program link error");
  }
}

void GlProgram::Detach(GlShader &shader) { glDetachShader(id_, shader.Id()); }

void GlProgram::GetActiveUniformLocations() {
  uniform_locations_.clear();
  GLint count;
  glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &count);
  GLint max_len;
  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);
  for (GLint i = 0; i < count; i++) {
    std::string name(max_len, '\0');
    GLsizei length;
    GLint size;
    GLenum type;
    glGetActiveUniform(id_, i, max_len, &length, &size, &type, name.data());
    name.resize(length);
    uniform_locations_[name] = glGetUniformLocation(id_, name.c_str());
  }
}
