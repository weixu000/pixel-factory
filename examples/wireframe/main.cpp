#include <glm/gtx/transform.hpp>
#include <iostream>

#include <PixelFactory/Entity.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/components/PointLight.h>
#include <PixelFactory/components/Trackball.h>
#include <PixelFactory/gl/GlContext.h>
#include <PixelFactory/gl/GlFramebuffer.h>
#include <PixelFactory/gl/GlRenderbuffer.h>
#include <PixelFactory/gl/GlTexture2D.h>
#include <PixelFactory/gl/GlVertexArray.h>
#include <PixelFactory/renderer/DeferredRenderer.h>
#include <PixelFactory/renderer/DrawOptions.h>

class Window : public GlContext {
public:
  Window() : GlContext(800, 600, "Empty Window") {
    scene_.SetEventHandler(handler_.get());
    camera_ =
        scene_.AddChild(Entity(glm::translate(glm::vec3(0.0f, 0.0f, 20.0f))))
            ->AddComponent(Camera(Width(), Height()));
    auto axes = scene_.AddChild(Entity());
    axes->AddComponent(Trackball(camera_));
    auto mesh = axes->AddComponent(Mesh::FromObjFile("meshes/cube.obj"));
    auto texture = std::make_shared<GlTexture2D>(
        GlTexture2D::FromImageFile("textures/container2.png", 3));
    texture->Bind();
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh->material_.diffuse = std::move(texture);
    texture = std::make_shared<GlTexture2D>(
        GlTexture2D::FromImageFile("textures/container2_specular.png", 1));
    texture->Bind();
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


