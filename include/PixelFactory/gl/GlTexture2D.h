#pragma once

#include <stb_image.h>

#include <PixelFactory/GL/GlTexture.h>
#include <PixelFactory/Image.h>

class GlContext;

class GlTexture2D : public GlTexture {
 public:
  void ImmutableStorage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    glTextureStorage2D(id_, levels, internalformat, width, height);
  }

  void FromImageFile(const std::string &image_path, int desired_channels) {
    Image image(image_path, desired_channels);
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
    ImmutableStorage(1, internalformat, image.width, image.height);
    Upload(0, 0, 0, image.width, image.height, format, GL_UNSIGNED_BYTE, image.data);
  }

  void Upload(GLint level,
              GLint xoffset, GLint yoffset,
              GLsizei width, GLsizei height,
              GLenum format, GLenum type, const void *pixels) {
    glTextureSubImage2D(id_, level, xoffset, yoffset, width, height, format, type, pixels);
  }

 private:
  friend class GlContext;
  explicit GlTexture2D(GLuint id) : GlTexture(TextureTarget::Texture2D, id) {}
};
