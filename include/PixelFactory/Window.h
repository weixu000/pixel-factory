#pragma once

#include <memory>
#include <string>
#include <tuple>

class Application;
class GLFWwindow;
class EventHandler;
class GlContext;

class Window {
 public:
  Window(int width, int height, const std::string &title);

  virtual ~Window();

  Window(const Window &) = delete;

  Window &operator=(const Window &) = delete;

  [[nodiscard]] int Width() const { return std::get<0>(FramebufferSize()); }

  [[nodiscard]] int Height() const { return std::get<1>(FramebufferSize()); }

  GlContext &Context() { return *context_; }

  bool ShouldClose();

 protected:
  GLFWwindow *window_;

  static Window *Retrieve(GLFWwindow *w);

  [[nodiscard]] std::tuple<int, int> FramebufferSize() const;

  virtual void Draw() {}

  std::unique_ptr<EventHandler> handler_;

  std::unique_ptr<GlContext> context_;

 private:
  void SetupCallbacks();

  friend class Application;
  void Loop();
};
