#pragma once

#include <glm/glm.hpp>

#include "PixelFactory/components/Component.h"

class UpdateEvent;
class KeyEvent;
class ScrollEvent;

class FreeMover : public Component {
 public:
  void OnUpdate(const UpdateEvent &event);

  void OnKeyPress(const KeyEvent &e);

  void OnKeyRelease(const KeyEvent &e);

 protected:
  void Start() override;

 private:
  bool forward_ = false, left_ = false, backward_ = false, right_ = false,
       up_ = false, down_ = false;
};
