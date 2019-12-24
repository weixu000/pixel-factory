#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include <PixelFactory/components/Trackball.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/EventHandler.h>

void Trackball::Start() {
  entity_->GetEventHandler()->Bind<CursorPositionEvent>("MouseMove",
                                                        [this](const CursorPositionEvent &e) { OnMouseMove(e); });
  entity_->GetEventHandler()->Bind<MouseButtonEvent>("MouseButtonPress",
                                                     [this](const MouseButtonEvent &e) { OnMouseButtonPress(e); });
  entity_->GetEventHandler()->Bind<MouseButtonEvent>("MouseButtonRelease",
                                                     [this](const MouseButtonEvent &e) { OnMouseButtonRelease(e); });
  entity_->GetEventHandler()->Bind<ScrollEvent>("Scroll",
                                                [this](const ScrollEvent &e) { OnScroll(e); });
}

void Trackball::OnMouseMove(const CursorPositionEvent &e) {
  auto x = e.x / camera_->Width();
  auto y = e.y / camera_->Height();
  if (started_) {
    auto cur = ViewportToTrackball(x, y);
    auto rad = glm::acos(glm::dot(initial_, cur));
    auto axis = glm::cross(initial_, cur);
    if (glm::length(axis) == 0) {
      return;
    }
    auto next = glm::rotate(rad, axis);
    auto delta = next * glm::transpose(orientation_);
    orientation_ = next;
    entity_->SetLocalTransform(delta * entity_->LocalTransform().matrix);
  }
}

void Trackball::OnMouseButtonPress(const MouseButtonEvent &e) {
  if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
    auto x = e.x / camera_->Width();
    auto y = e.y / camera_->Height();
    initial_ = ViewportToTrackball(x, y);
    started_ = true;
  }
}

void Trackball::OnMouseButtonRelease(const MouseButtonEvent &e) {
  if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
    started_ = false;
    orientation_ = glm::mat4(1.0f);
  }
}

void Trackball::OnScroll(const ScrollEvent &e) {
  entity_->SetLocalTransform(
      glm::scale(glm::vec3(glm::max(0.0f, 1 + 0.1f * e.y_offset))) * entity_->LocalTransform().matrix);
}

glm::vec3 Trackball::ViewportToTrackball(float x, float y) {
  glm::vec3 ret(2 * x - 1,
                -(2 * y - 1),
                0.0f);
  auto z2 = 1 - ret.x * ret.x - ret.y * ret.y;
  if (z2 >= 0) {
    ret.z = glm::sqrt(z2);
    return ret;
  } else {
    return glm::normalize(ret);
  }
}
