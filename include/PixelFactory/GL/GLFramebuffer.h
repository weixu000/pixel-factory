#pragma once

#include <glad/glad.h>

#include <PixelFactory/GL/GLTexture2D.h>
#include <PixelFactory/GL/GLRenderbuffer.h>

class GLFramebuffer {
 public:
  enum class Target : GLenum {
    Unbound = 0,
    Framebuffer = GL_FRAMEBUFFER,
    ReadFramebuffer = GL_READ_FRAMEBUFFER,
    DrawFramebuffer = GL_DRAW_FRAMEBUFFER,
  };

  GLFramebuffer() {
    glGenFramebuffers(1, &id_);
  }

  ~GLFramebuffer() {
    glDeleteFramebuffers(1, &id_);
  }

  void Bind(Target target = Target::Framebuffer) {
    target_ = target;
    glBindFramebuffer(static_cast<GLenum>(target_), id_);
  }

  void Unbind() {
    glBindFramebuffer(static_cast<GLenum>(target_), 0);
    target_ = Target::Unbound;
  }

  void Attach(GLenum attachment, const GLTexture2D &texture, GLint level = 0) {
    glFramebufferTexture2D(static_cast<GLenum>(target_), attachment, GL_TEXTURE_2D, texture.id_, level);
  }

  void Attach(GLenum attachment, const GLRenderbuffer &renderbuffer) {
    glFramebufferRenderbuffer(static_cast<GLenum>(target_), attachment, GL_RENDERBUFFER, renderbuffer.id_);
  }

  bool IsComplete() {
    return glCheckFramebufferStatus(static_cast<GLenum>(target_)) == GL_FRAMEBUFFER_COMPLETE;
  }

 private:
  GLuint id_ = 0U;
  Target target_ = Target::Unbound;
};
