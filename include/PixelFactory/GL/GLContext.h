#pragma once

#include <tuple>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EventHandler;

class GLContext {
 public:
  GLContext(int width, int height, const std::string &title);

  virtual ~GLContext() {
    glfwDestroyWindow(window_);
  }

  GLContext(const GLContext &) = delete;

  GLContext &operator=(const GLContext &) = delete;

  [[nodiscard]] int Width() const { return std::get<0>(FramebufferSize()); }

  [[nodiscard]] int Height() const { return std::get<1>(FramebufferSize()); }

  void Loop();

 protected:
  GLFWwindow *window_;

  static GLContext *Retrieve(GLFWwindow *w) { return reinterpret_cast<GLContext *>(glfwGetWindowUserPointer(w)); }

  [[nodiscard]] std::tuple<int, int> FramebufferSize() const {
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    return std::make_tuple(width, height);
  }

  virtual void Update() {}

  virtual void Draw() {}

  std::unique_ptr<EventHandler> handler_;

 private:
  void SetupCallbacks();
};
