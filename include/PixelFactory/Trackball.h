#pragma once

#include <glm/glm.hpp>
#include <PixelFactory/Component.h>

class Camera;

class Trackball : public Component {
 public:
  explicit Trackball(Camera *cam) : camera_(cam) {}

  void OnMouseMove(const CursorPositionEvent &e) override;

  void OnMouseButtonPress(const MouseButtonEvent &e) override;

  void OnMouseButtonRelease(const MouseButtonEvent &e) override;

  void OnScroll(const ScrollEvent &e) override;

 private:
  static glm::vec3 ViewportToTrackball(float x, float y);

  bool started_ = false;
  glm::mat4 orientation_{1.0f};
  glm::vec3 initial_;

  Camera *camera_;
};


