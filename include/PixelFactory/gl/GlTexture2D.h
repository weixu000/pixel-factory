#pragma once

#include <stb_image.h>

#include <PixelFactory/gl/GlTexture.h>

class GlTexture2D : public GlTexture {
 public:
  GlTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
      : GlTexture(Target::Texture2D),
        width_(width), height_(height) {
    glTextureStorage2D(id_, levels, internalformat, width, height);
  }

  static GlTexture2D FromImageFile(const std::string &image_path, int desired_channels) {
    GLenum format, internalformat;
    switch (desired_channels) {
      case 1:format = GL_RED;
        internalformat = GL_R8;
        break;
      case 3:format = GL_RGB;
        internalformat = GL_RGB8;
        break;
      case 4:format = GL_RGBA;
        internalformat = GL_RGBA8;
        break;
      default:throw std::invalid_argument("Wrong desired_channels");
    }
    int width, height, nr_channels;
    auto data = stbi_load(image_path.c_str(), &width, &height, &nr_channels, desired_channels);
    if (data == nullptr) {
      throw std::runtime_error(std::string("stbi_load() failure: ") + stbi_failure_reason());
    }

    GlTexture2D tex(1, internalformat, width, height);
    tex.Upload(0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tex;
  }

  void Upload(GLint level,
              GLint xoffset, GLint yoffset,
              GLsizei width, GLsizei height,
              GLenum format, GLenum type, const void *pixels) {
    glTextureSubImage2D(id_, level, xoffset, yoffset, width, height, format, type, pixels);
  }

 private:
  const GLsizei width_, height_;
};
