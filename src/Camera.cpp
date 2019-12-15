#include <glm/gtx/transform.hpp>

#include <PixelFactory/Camera.h>
#include <PixelFactory/Entity.h>

void Camera::Update() {
  auto cam = entity_->WorldTransform().matrix;
  view_ = glm::inverse(cam);
  eye_ = glm::vec3(cam[3]);
}

void Camera::Resize(float w, float h) {
  width_ = w;
  height_ = h;
  projection_ = glm::perspective(glm::radians(fovy_),
                                 width_ / height_,
                                 z_near_, z_far_);
}
