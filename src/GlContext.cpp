#include <iostream>
#include <stdexcept>

#include <PixelFactory/GL/GlContext.h>

GlContext::GlContext()
    : null_buffer_(0U), null_renderbuffer_(0U), null_texture2d_(0U),
      null_texture_cubemap_(0U), default_framebuffer_(0U),
      null_vertex_array_(0U) {
  if (!gladLoadGL()) {
    throw std::runtime_error("Failed to initialize GLAD.");
  }
  // Get info of GPU and supported OpenGL version.
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl
            << "OpenGL version supported: " << glGetString(GL_VERSION)
            << std::endl
            << "Supported GLSL version is: "
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GlContext::Clear(bool color, bool depth) {
  GLbitfield mask = 0;
  if (color) {
    mask |= GL_COLOR_BUFFER_BIT;
  }
  if (depth) {
    mask |= GL_DEPTH_BUFFER_BIT;
  }
  glClear(mask);
}

void GlContext::Viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  glViewport(x, y, width, height);
}

GlBuffer GlContext::CreateBuffer() {
  GLuint id;
  glCreateBuffers(1, &id);
  return GlBuffer(id);
}

GlRenderbuffer GlContext::CreateRenderbuffer() {
  GLuint id;
  glCreateRenderbuffers(1, &id);
  return GlRenderbuffer(id);
}

GlTexture2D GlContext::CreateTexture2D() {
  GLuint id;
  glCreateTextures(GL_TEXTURE_2D, 1, &id);
  return GlTexture2D(id);
}

GlTextureCubemap GlContext::CreateTextureCubemap() {
  GLuint id;
  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
  return GlTextureCubemap(id);
}

GlFramebuffer GlContext::CreateFramebuffer() {
  GLuint id;
  glCreateFramebuffers(1, &id);
  return GlFramebuffer(id);
}

GlVertexArray GlContext::CreateVertexArray() {
  GLuint id;
  glCreateVertexArrays(1, &id);
  return GlVertexArray(id);
}

void GlContext::Bind(BufferTarget target, GlBuffer &buffer) {
  glBindBuffer(static_cast<GLenum>(target), buffer.Id());
}

void GlContext::Bind(GlRenderbuffer &renderbuffer) {
  glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer.Id());
}

void GlContext::Bind(FramebufferTarget target, GlFramebuffer &framebuffer) {
  glBindFramebuffer(static_cast<GLenum>(target), framebuffer.Id());
}

void GlContext::Bind(GlVertexArray &vertex_array) {
  glBindVertexArray(vertex_array.Id());
}

void GlContext::Bind(GlTexture &texture) {
  glBindTexture(static_cast<GLenum>(texture.Target()), texture.Id());
}
