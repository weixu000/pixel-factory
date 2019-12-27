#include <glm/gtx/transform.hpp>
#include <iostream>

#include <PixelFactory/Application.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/GL/GlContext.h>
#include <PixelFactory/GL/GlFramebuffer.h>
#include <PixelFactory/GL/GlTexture2D.h>
#include <PixelFactory/GL/GlVertexArray.h>
#include <PixelFactory/Window.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/components/FreeMover.h>
#include <PixelFactory/components/FreeRotator.h>
#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/components/PointLight.h>
#include <PixelFactory/renderer/DeferredRenderer.h>
#include <PixelFactory/renderer/DrawOptions.h>

class MyWindow : public Window {
public:
  MyWindow() : Window(800, 600, "Mesh Viewer") {
    handler_->Bind<ResizeEvent>("Resize",
                                [this](const ResizeEvent &e) { OnResize(e); });
    scene_.SetEventHandler(handler_.get());

    CreateScene();

    renderer_ =
        std::make_unique<DeferredRenderer>(*context_, Width(), Height());
    renderer_->Collect(scene_);
  }

protected:
  void Draw() override {
    renderer_->GeometryPass({*camera_});
    renderer_->ShadowPass();
    renderer_->LightingPass({*camera_});
  }

private:
  Entity scene_;
  Camera *camera_;
  std::unique_ptr<DeferredRenderer> renderer_;

  void CreateScene() {
    auto cam_obj =
        scene_.AddChild(Entity(glm::translate(glm::vec3(0.0f, 0.0f, 10.0f))));
    camera_ = cam_obj->AddComponent(Camera(Width(), Height()));
    cam_obj->AddComponent(FreeMover());
    cam_obj->AddComponent(FreeRotator(camera_));

    auto quad = Mesh::FromObjFile(*context_, "meshes/quad.obj");
    auto texture = std::make_shared<GlTexture2D>(context_->CreateTexture2D());
    texture->FromImageFile("textures/plain.png", 3);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    quad.material_.diffuse = std::move(texture);
    texture = std::make_shared<GlTexture2D>(context_->CreateTexture2D());
    texture->FromImageFile("textures/black.png", 1);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    quad.material_.specular = std::move(texture);
    scene_.AddChild(Entity(glm::scale(glm::vec3(25.0f))))
        ->AddComponent(std::move(quad));

    auto mesh = Mesh::FromObjFile(*context_, "meshes/cube.obj");
    texture = std::make_shared<GlTexture2D>(context_->CreateTexture2D());
    texture->FromImageFile("textures/container2.png", 3);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh.material_.diffuse = std::move(texture);
    texture = std::make_shared<GlTexture2D>(context_->CreateTexture2D());
    texture->FromImageFile("textures/container2_specular.png", 1);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    mesh.material_.specular = std::move(texture);

    for (int i = -3; i <= 3; ++i) {
      for (int j = -3; j <= 3; ++j) {
        scene_.AddChild(Entity(glm::translate(glm::vec3(3 * i, 0.5f, 3 * j))))
            ->AddComponent(Mesh(mesh));
      }
    }

    for (int i = -1; i < 2; i += 2) {
      for (int j = -1; j < 2; j += 2) {
        auto light = PointLight(*context_);
        light.color = glm::vec3(20.0f);
        light.attenuation = 0.5f;
        light.fall_off = 50.0f;
        scene_
            .AddChild(Entity(glm::translate(glm::vec3(10 * i, 5.0f, 10 * j))))
            ->AddComponent(std::move(light));
      }
    }
  }

  void OnResize(const ResizeEvent &e) {
    if (e.width && e.height) {
      context_->Viewport(0, 0, e.width, e.height);
      camera_->Resize(e.width, e.height);
      renderer_ =
          std::make_unique<DeferredRenderer>(*context_, Width(), Height());
      renderer_->Collect(scene_);
    }
  }
};

int main() {
  Application app;
  MyWindow window;
  return app.Run();
}
