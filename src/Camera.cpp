#include <glm/gtx/transform.hpp>

#include <PixelFactory/components/Camera.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/EventHandler.h>

void Camera::OnUpdate(const UpdateEvent &event) {
  view_ = glm::inverse(entity_->WorldTransform().matrix);
  eye_ = entity_->WorldTransform().Translation();
}

void Camera::Resize(float w, float h) {
  width_ = w;
  height_ = h;
  projection_ =
      glm::perspective(glm::radians(fovy_), width_ / height_, z_near_, z_far_);
}

void Camera::Start() {
  entity_->GetEventHandler()->Bind<UpdateEvent>(
      "Update", [this](const UpdateEvent &e) { OnUpdate(e); });
}
