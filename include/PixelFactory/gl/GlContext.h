#pragma once

#include <tuple>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EventHandler;

class GlContext {
 public:
  GlContext(int width, int height, const std::string &title);

  virtual ~GlContext() {
    glfwDestroyWindow(window_);
  }

  GlContext(const GlContext &) = delete;

  GlContext &operator=(const GlContext &) = delete;

  [[nodiscard]] int Width() const { return std::get<0>(FramebufferSize()); }

  [[nodiscard]] int Height() const { return std::get<1>(FramebufferSize()); }

  void Loop();

 protected:
  GLFWwindow *window_;

  static GlContext *Retrieve(GLFWwindow *w) { return reinterpret_cast<GlContext *>(glfwGetWindowUserPointer(w)); }

  [[nodiscard]] std::tuple<int, int> FramebufferSize() const {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    return std::make_tuple(width, height);
  }

  virtual void Draw() {}

  std::unique_ptr<EventHandler> handler_;

 private:
  void SetupCallbacks();
};
