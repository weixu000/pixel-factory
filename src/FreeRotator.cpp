#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <PixelFactory/components/FreeRotator.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/Entity.h>
#include <PixelFactory/components/Camera.h>
#include <PixelFactory/EventHandler.h>

void FreeRotator::Start() {
  entity_->GetEventHandler()->Bind<CursorPositionEvent>("MouseMove",
                                                        [this](const CursorPositionEvent &e) { OnMouseMove(e); });
  entity_->GetEventHandler()->Bind<MouseButtonEvent>("MouseButtonPress",
                                                     [this](const MouseButtonEvent &e) { OnMouseButtonPress(e); });
  entity_->GetEventHandler()->Bind<MouseButtonEvent>("MouseButtonRelease",
                                                     [this](const MouseButtonEvent &e) { OnMouseButtonRelease(e); });
}

void FreeRotator::OnMouseMove(const CursorPositionEvent &e) {
  if (startedRotate) {
    glm::vec2 current(e.x, e.y);
    auto delta = current - previous;
    delta.x *= 0.001f;
    delta.y *= 0.001f;

    eulerX = std::remainder(eulerX + delta.x, 2 * glm::pi<float>());
    eulerY = std::remainder(eulerY + delta.y, 2 * glm::pi<float>());
    auto rotation = glm::eulerAngleYX(eulerX, eulerY);
    entity_->SetLocalTransform(glm::translate(entity_->LocalTransform().Translation()) * rotation);
    previous = current;
  }
}

void FreeRotator::OnMouseButtonPress(const MouseButtonEvent &e) {
  if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
    startedRotate = true;
    previous = glm::vec2(e.x, e.y);
  }
}

void FreeRotator::OnMouseButtonRelease(const MouseButtonEvent &e) {
  if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
    startedRotate = false;
  }
}






