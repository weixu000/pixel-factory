#include <glm/gtx/transform.hpp>
#include <iostream>

#include <PixelFactory/Camera.h>
#include <PixelFactory/DeferredRenderer.h>
#include <PixelFactory/DrawOptions.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/GL/GLContext.h>
#include <PixelFactory/GL/GLFramebuffer.h>
#include <PixelFactory/GL/GLRenderbuffer.h>
#include <PixelFactory/GL/GLTexture2D.h>
#include <PixelFactory/GL/GLVertexArray.h>
#include <PixelFactory/Mesh.h>
#include <PixelFactory/PointLight.h>
#include <PixelFactory/Trackball.h>

class Window : public GLContext {
public:
  Window() : GLContext(800, 600, "Empty Window") {
    scene_.SetEventHandler(handler_.get());
    camera_ =
        scene_.AddChild(Entity(glm::translate(glm::vec3(0.0f, 0.0f, 20.0f))))
            ->AddComponent(Camera(Width(), Height()));
    auto axes = scene_.AddChild(Entity());
    axes->AddComponent(Trackball(camera_));
    auto mesh = axes->AddComponent(Mesh::FromObjFile("meshes/cube.obj"));
    auto texture = std::make_shared<GLTexture2D>();
    texture->Bind();
    texture->Upload("textures/container2.png", 3);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh->material_.diffuse = std::move(texture);
    texture = std::make_shared<GLTexture2D>();
    texture->Bind();
    texture->Upload("textures/container2_specular.png", 1);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh->material_.specular = std::move(texture);

    auto light = scene_.AddChild(Entity(glm::translate(glm::vec3(5.0f))))
                     ->AddComponent(PointLight());
    light->color = glm::vec3(5.0f);
    light->attenuation = 0.2f;
    light->fall_off = 10.0f;

    renderer_ = std::make_unique<DeferredRenderer>(Width(), Height());
    renderer_->Collect(scene_);

    handler_->Bind<ResizeEvent>("Resize",
                                [this](const ResizeEvent &e) { OnResize(e); });
  }

protected:
  void Draw() override {
    renderer_->GeometryPass({*camera_});
    renderer_->LightingPass({*camera_});
  }

private:
  Entity scene_;
  Camera *camera_;
  std::unique_ptr<DeferredRenderer> renderer_;

  void OnResize(const ResizeEvent &e) {
    if (e.width && e.height) {
      glViewport(0, 0, e.width, e.height);
      camera_->Resize(e.width, e.height);
      renderer_ = std::make_unique<DeferredRenderer>(Width(), Height());
      renderer_->Collect(scene_);
    }
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


