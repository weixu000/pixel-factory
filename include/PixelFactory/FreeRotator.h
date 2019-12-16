#pragma once

#include <glm/glm.hpp>
#include <PixelFactory/Component.h>

class Camera;
class MouseButtonEvent;
class CursorPositionEvent;

class FreeRotator : public Component {
 public:
  explicit FreeRotator(Camera *cam) : camera_(cam) {}

  void OnMouseMove(const CursorPositionEvent &e);

  void OnMouseButtonPress(const MouseButtonEvent &e);

  void OnMouseButtonRelease(const MouseButtonEvent &e);

 protected:
  void Start() override;

 private:
  bool startedRotate = false;
  glm::vec2 previous = glm::vec2(0.0f);
  float eulerX = 0.0f, eulerY = 0.0f;

  Camera *camera_;
};


