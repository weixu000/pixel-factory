#include "PixelFactory/components/FreeMover.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>

#include "PixelFactory/Entity.h"
#include "PixelFactory/Event.h"
#include "PixelFactory/EventHandler.h"

namespace {
const int forwardKey = GLFW_KEY_W, backwardKey = GLFW_KEY_S,
          leftKey = GLFW_KEY_A, rightKey = GLFW_KEY_D, upKey = GLFW_KEY_Q,
          downKey = GLFW_KEY_E;
}

void FreeMover::Start() {
  entity_->GetEventHandler()->Bind<KeyEvent>(
      "KeyPress", [this](const KeyEvent &e) { OnKeyPress(e); });
  entity_->GetEventHandler()->Bind<KeyEvent>(
      "KeyRelease", [this](const KeyEvent &e) { OnKeyRelease(e); });
  entity_->GetEventHandler()->Bind<UpdateEvent>(
      "Update", [this](const UpdateEvent &e) { OnUpdate(e); });
}

void FreeMover::OnUpdate(const UpdateEvent &event) {
  auto movement = glm::vec3(0.0f);
  movement += glm::vec3(entity_->LocalTransform().matrix[2]) *
              (float(backward_) - float(forward_));
  movement += glm::vec3(entity_->LocalTransform().matrix[0]) *
              (float(right_) - float(left_));
  movement += glm::vec3(entity_->LocalTransform().matrix[1]) *
              (float(up_) - float(down_));
  movement *= 10.0f * event.delta_time;
  entity_->SetLocalTransform(glm::translate(movement) *
                             entity_->LocalTransform().matrix);
}

void FreeMover::OnKeyPress(const KeyEvent &e) {
  switch (e.key) {
    case forwardKey:
      forward_ = true;
      break;
    case backwardKey:
      backward_ = true;
      break;
    case leftKey:
      left_ = true;
      break;
    case rightKey:
      right_ = true;
      break;
    case upKey:
      up_ = true;
      break;
    case downKey:
      down_ = true;
      break;
  }
}

void FreeMover::OnKeyRelease(const KeyEvent &e) {
  switch (e.key) {
    case forwardKey:
      forward_ = false;
      break;
    case backwardKey:
      backward_ = false;
      break;
    case leftKey:
      left_ = false;
      break;
    case rightKey:
      right_ = false;
      break;
    case upKey:
      up_ = false;
      break;
    case downKey:
      down_ = false;
      break;
  }
}
