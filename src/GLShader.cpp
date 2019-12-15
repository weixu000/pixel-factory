#include <iostream>
#include <fstream>

#include <PixelFactory/GL/GLShader.h>

namespace {
enum class ShaderType : GLenum {
  VectexShader = GL_VERTEX_SHADER,
  FragementShader = GL_FRAGMENT_SHADER,
  GeometryShader = GL_GEOMETRY_SHADER
};

GLuint LoadSingleShader(const std::string &shader_file_path, ShaderType type) {
  // Create a shader id.
  GLuint id = 0;
  id = glCreateShader(static_cast<GLenum>(type));

  // Try to read shader codes from the shader file.
  std::string code;
  std::ifstream stream(shader_file_path, std::ios::in);
  if (stream.is_open()) {
    std::string line;
    while (getline(stream, line))
      code += "\n" + line;
    stream.close();
  } else {
    throw std::runtime_error("Cannot open " + shader_file_path);
  }

  // Compile Shader.
  std::cout << "Compiling shader: " << shader_file_path << std::endl;
  char const *source = code.c_str();
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);

  // Check Shader.
  GLint result = GL_FALSE;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  int log_length;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0) {
    std::string msg(log_length, '\0');
    glGetShaderInfoLog(id, log_length, nullptr, msg.data());
    std::cerr << msg << std::endl;
  }

  if (result != GL_TRUE) {
    throw std::runtime_error("Shader compilation error: " + shader_file_path);
  }

  return id;
}
}

void GLShader::CompileProgram(const std::string &vertex_file_path,
                              const std::string &fragment_file_path,
                              const std::string &geometry_file_path) {
// Create the vertex shader and fragment shader.
  auto vertex_id = LoadSingleShader(vertex_file_path, ShaderType::VectexShader);
  auto fragment_id = LoadSingleShader(fragment_file_path, ShaderType::FragementShader);
  auto geometry_id = geometry_file_path.empty() ? 0 : LoadSingleShader(geometry_file_path, ShaderType::GeometryShader);

  // Link the program.
  std::cout << "Linking program" << std::endl;
  glAttachShader(id_, vertex_id);
  glAttachShader(id_, fragment_id);
  if (geometry_id) glAttachShader(id_, geometry_id);
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
    std::cerr << msg << std::endl;
  }

  if (result != GL_TRUE) {
    throw std::runtime_error(
        "Program link error: " + vertex_file_path + ", " + fragment_file_path + ", " + geometry_file_path);
  }

  // Detach and delete the shaders as they are no longer needed.
  glDetachShader(id_, vertex_id);
  glDetachShader(id_, fragment_id);
  if (geometry_id) glDetachShader(id_, geometry_id);
  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);
  if (geometry_id) glDeleteShader(geometry_id);
}

GLShader::GLShader(const std::string &vertex_file_path,
                   const std::string &fragment_file_path,
                   const std::string &geometry_file_path)
    : GLShader() {
  CompileProgram(vertex_file_path, fragment_file_path, geometry_file_path);

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
