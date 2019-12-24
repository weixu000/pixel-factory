#include <iostream>
#include <glm/gtx/transform.hpp>

#include <PixelFactory/GL/GLContext.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/components/FreeMover.h>
#include <PixelFactory/components/FreeRotator.h>
#include <PixelFactory/renderer/DrawOptions.h>
#include <PixelFactory/GL/GLVertexArray.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/renderer/DeferredRenderer.h>
#include <PixelFactory/GL/GLFramebuffer.h>
#include <PixelFactory/components/PointLight.h>

class Window : public GLContext {
 public:
  Window() : GLContext(800, 600, "Mesh Viewer") {
    handler_->Bind<ResizeEvent>("Resize", [this](const ResizeEvent &e) { OnResize(e); });
    scene_.SetEventHandler(handler_.get());

    CreateScene();

    renderer_ = std::make_unique<DeferredRenderer>(Width(), Height());
    renderer_->Collect(scene_);
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

  void CreateScene() {
    auto cam_obj = scene_.AddChild(Entity(glm::translate(glm::vec3(0.0f, 0.0f, 10.0f))));
    camera_ = cam_obj->AddComponent(Camera(Width(), Height()));
    cam_obj->AddComponent(FreeMover());
    cam_obj->AddComponent(FreeRotator(camera_));

    auto quad = Mesh::FromObjFile("meshes/quad.obj");
    auto texture = std::make_shared<GLTexture2D>();
    texture->Bind();
    texture->Upload("textures/plain.png", 3);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    quad.material_.diffuse = std::move(texture);
    texture = std::make_shared<GLTexture2D>();
    texture->Bind();
    texture->Upload("textures/black.png", 1);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    quad.material_.specular = std::move(texture);
    scene_.AddChild(Entity(glm::scale(glm::vec3(25.0f))))->AddComponent(std::move(quad));

    auto mesh = Mesh::FromObjFile("meshes/cube.obj");
    texture = std::make_shared<GLTexture2D>();
    texture->Bind();
    texture->Upload("textures/container2.png", 3);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh.material_.diffuse = std::move(texture);
    texture = std::make_shared<GLTexture2D>();
    texture->Bind();
    texture->Upload("textures/container2_specular.png", 1);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh.material_.specular = std::move(texture);

    for (int i = -5; i < 5; ++i) {
      for (int j = -5; j < 5; ++j) {
        scene_.AddChild(Entity(glm::translate(glm::vec3(2 * i, 0.5f, 2 * j))))
            ->AddComponent(Mesh(mesh));
      }
    }

    auto light = PointLight();
    light.color = glm::vec3(10.0f);
    light.attenuation = 0.5f;
    light.fall_off = 8.0f;
    for (int i = -1; i < 1; ++i) {
      for (int j = -1; j < 1; ++j) {
        scene_.AddChild(Entity(glm::translate(glm::vec3(6 * i, 4.0f, 6 * j))))
            ->AddComponent(PointLight(light));
      }
    }
  }

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


