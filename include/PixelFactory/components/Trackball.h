#pragma once

#include <glm/glm.hpp>
#include <PixelFactory/components/Component.h>

class Camera;
class MouseButtonEvent;
class CursorPositionEvent;
class ScrollEvent;

class Trackball : public Component {
 public:
  explicit Trackball(Camera *cam) : camera_(cam) {}

  void OnMouseMove(const CursorPositionEvent &e);

  void OnMouseButtonPress(const MouseButtonEvent &e);

  void OnMouseButtonRelease(const MouseButtonEvent &e);

  void OnScroll(const ScrollEvent &e);

 protected:
  void Start() override;

 private:
  static glm::vec3 ViewportToTrackball(float x, float y);

  bool started_ = false;
  glm::mat4 orientation_{1.0f};
  glm::vec3 initial_;

  Camera *camera_;
};
