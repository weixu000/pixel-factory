#include <iostream>
#include <glm/gtx/transform.hpp>

#include <PixelFactory/GLContext.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/Camera.h>
#include <PixelFactory/Mesh.h>
#include <PixelFactory/Trackball.h>
#include <PixelFactory/DrawOptions.h>
#include <PixelFactory/GLVertexArray.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/Event.h>

class Window : public GLContext {
 public:
  Window() : GLContext(800, 600, "Empty Window") {
    scene_.SetEventHandler(handler_.get());
    camera_ = scene_.AddChild(Entity(glm::translate(glm::vec3(0.0f, 0.0f, 20.0f))))
        ->AddComponent(Camera(Width(), Height()));
    auto axes = scene_.AddChild(Entity());
    axes->AddComponent(Trackball(camera_));
    auto mesh = axes->AddComponent(Mesh::FromObjFile("meshes/bunny.obj"));

    handler_->Bind<ResizeEvent>("Resize", [this](const ResizeEvent &e) { OnResize(e); });
  }

 protected:
  void Draw() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene_.Draw(DrawOptions{*camera_});
  }

  void Update() override {
    scene_.Update();
  }

 private:
  Entity scene_;
  Camera *camera_;

  void OnResize(const ResizeEvent &e) {
    glViewport(0, 0, e.width, e.height);
    camera_->Resize(e.width, e.height);
  }
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


