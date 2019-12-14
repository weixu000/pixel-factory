#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tuple>

class GLContext {
 public:
  GLContext(int width, int height, const std::string &title);

  virtual ~GLContext() {
    glfwDestroyWindow(window_);
  }

  GLContext(const GLContext &) = delete;

  GLContext &operator=(const GLContext &) = delete;

  [[nodiscard]] int Width() const { return std::get<0>(WindowSize()); }

  [[nodiscard]] int Height() const { return std::get<1>(WindowSize()); }

  void Loop();

 protected:
  GLFWwindow *window_;

  static GLContext *Retrieve(GLFWwindow *w) { return reinterpret_cast<GLContext *>(glfwGetWindowUserPointer(w)); }

  [[nodiscard]] std::tuple<int, int> WindowSize() const {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    return std::make_tuple(width, height);
  }

  virtual void Update() {}

  virtual void Draw() {}

  virtual void ResizeCallback(int width, int height) {}

  virtual void KeyCallback(int key, int scancode, int action, int mods) {}

  virtual void MouseButtonCallback(int button, int action, int mods) {}

  virtual void CursorPosCallback(double x, double y) {}

  virtual void ScrollCallback(double xoffset, double yoffset) {}

 private:
  void SetupCallbacks();
};
