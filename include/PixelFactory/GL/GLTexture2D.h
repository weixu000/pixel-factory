#pragma once

#include <stb_image.h>

#include <PixelFactory/GL/GLTexture.h>

class GLTexture2D : public GLTexture {
 public:
  GLTexture2D() : GLTexture(Target::Texture2D) {}

  void Allocate(GLint level, GLenum internalformat,
                GLsizei width, GLsizei height,
                GLenum format, GLenum type,
                const void *data = nullptr) {
    glTexImage2D(static_cast<GLenum>(target_), level, internalformat, width, height, 0, format, type, data);
  }

  void Upload(const std::string &image_path, int desired_channels) {
    GLenum format;
    switch (desired_channels) {
      case 1:format = GL_RED;
        break;
      case 3:format = GL_RGB;
        break;
      case 4:format = GL_RGBA;
        break;
      default:throw std::invalid_argument("Wrong desired_channels");
    }
    int width, height, nr_channels;
    auto data = stbi_load(image_path.c_str(), &width, &height, &nr_channels, desired_channels);
    if (data == nullptr) {
      throw std::runtime_error(std::string("stbi_load() failure: ") + stbi_failure_reason());
    }
    Allocate(0, format, width, height, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

 private:
  friend class GLFramebuffer;
};
