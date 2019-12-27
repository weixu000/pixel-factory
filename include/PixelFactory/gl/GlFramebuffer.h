#pragma once

#include <glad/glad.h>

#include <initializer_list>

#include "PixelFactory/gl/GlRenderbuffer.h"
#include "PixelFactory/gl/GlTexture2D.h"
#include "PixelFactory/gl/GlTextureCubemap.h"

class GlContext;

enum class FramebufferTarget : GLenum {
  Unbound = 0,
  Framebuffer = GL_FRAMEBUFFER,
  ReadFramebuffer = GL_READ_FRAMEBUFFER,
  DrawFramebuffer = GL_DRAW_FRAMEBUFFER,
};

class GlFramebuffer {
 public:
  GlFramebuffer(GlFramebuffer &&other) noexcept : id_(other.id_) {
    other.id_ = 0U;
  }

  ~GlFramebuffer() { glDeleteFramebuffers(1, &id_); }

  GlFramebuffer &operator=(GlFramebuffer &&other) noexcept {
    assert(this != &other);
    glDeleteFramebuffers(1, &id_);
    id_ = other.id_;
    other.id_ = 0U;
    return *this;
  }

  [[nodiscard]] GLuint Id() const { return id_; }

  void Attach(GLenum attachment, const GlTexture2D &texture, GLint level = 0) {
    glNamedFramebufferTexture(id_, attachment, texture.Id(), level);
  }

  void Attach(GLenum attachment, const GlRenderbuffer &renderbuffer) {
    glNamedFramebufferRenderbuffer(id_, attachment, GL_RENDERBUFFER,
                                   renderbuffer.Id());
  }

  void Attach(GLenum attachment, const GlTextureCubemap &texture,
              GLint level = 0) {
    glNamedFramebufferTexture(id_, attachment, texture.Id(), level);
  }

  void ReadBuffer(GLenum mode) { glNamedFramebufferReadBuffer(id_, mode); }

  void DrawBuffers(std::initializer_list<GLenum> bufs) {
    glNamedFramebufferDrawBuffers(id_, bufs.size(), bufs.begin());
  }

  bool IsComplete(FramebufferTarget target = FramebufferTarget::Framebuffer) {
    return glCheckNamedFramebufferStatus(id_, static_cast<GLenum>(target)) ==
           GL_FRAMEBUFFER_COMPLETE;
  }

 private:
  GLuint id_ = 0U;

  friend class GlContext;
  explicit GlFramebuffer(GLuint id) : id_(id) {}
};
