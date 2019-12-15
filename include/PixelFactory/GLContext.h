#pragma once

#include <tuple>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ResizeEvent;
class KeyEvent;
class MouseButtonEvent;
class CursorPositionEvent;
class ScrollEvent;

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

  virtual void OnResize(const ResizeEvent &e) {}

  virtual void OnKeyPress(const KeyEvent &e) {}

  virtual void OnKeyRelease(const KeyEvent &e) {}

  virtual void OnMouseMove(const CursorPositionEvent &e) {}

  virtual void OnMouseButtonPress(const MouseButtonEvent &e) {}

  virtual void OnMouseButtonRelease(const MouseButtonEvent &e) {}

  virtual void OnScroll(const ScrollEvent &e) {}

 private:
  void SetupCallbacks();
};
