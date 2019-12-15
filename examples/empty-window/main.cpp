#include <iostream>
#include <glm/gtx/transform.hpp>
#include <PixelFactory/GLContext.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/Camera.h>
#include <PixelFactory/Axes.h>
#include <PixelFactory/Trackball.h>
#include <PixelFactory/DrawOptions.h>

class Window : public GLContext {
 public:
  Window() : GLContext(800, 600, "Empty Window") {
    camera_ = scene_.AddChild(Entity(glm::translate(glm::vec3(0.0f, 0.0f, 20.0f))))
        ->AddComponent(Camera(Width(), Height()));
    auto axes = scene_.AddChild(Entity());
    axes->AddComponent(Axes());
    axes->AddComponent(Trackball(camera_));
  }

  void Draw() override {
    glClear(GL_COLOR_BUFFER_BIT);
    scene_.Draw(DrawOptions{*camera_});
  }

  void Update() override {
    scene_.Update();
  }

  virtual void OnResize(const ResizeEvent &e) {
    int w, h;
    glfwGetFramebufferSize(window_, &w, &h);
    glViewport(0, 0, w, h);
    camera_->Resize(w, h);
  }

  void OnMouseMove(const CursorPositionEvent &e) override { scene_.OnMouseMove(e); }

  void OnMouseButtonPress(const MouseButtonEvent &e) override { scene_.OnMouseButtonPress(e); }

  void OnMouseButtonRelease(const MouseButtonEvent &e) override { scene_.OnMouseButtonRelease(e); }

  void OnScroll(const ScrollEvent &e) override { scene_.OnScroll(e); }

 private:
  Entity scene_;
  Camera *camera_;
};

void ErrorCallback(int error, const char *description) {
  // Print error.
  std::cerr << description << std::endl;
  exit(EXIT_FAILURE);
}

void SetupGLFW() {
  // Initialize GLFW.
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Setup error callback.
  glfwSetErrorCallback(ErrorCallback);
}

int main() {
  SetupGLFW();

  Window window;
  window.Loop();
}


