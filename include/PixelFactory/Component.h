#pragma once

class Entity;
class DrawOptions;
class ResizeEvent;
class KeyEvent;
class MouseButtonEvent;
class CursorPositionEvent;
class ScrollEvent;

class Component {
 public:
  void Attach(Entity *entity) {
    entity_ = entity;
    Start();
  }

  void Detach() {
    entity_ = nullptr;
    Stop();
  }

  virtual void Draw(const DrawOptions &options) {}

  virtual void Update() {}

  virtual void OnKeyPress(const KeyEvent &e) {}

  virtual void OnKeyRelease(const KeyEvent &e) {}

  virtual void OnMouseMove(const CursorPositionEvent &e) {}

  virtual void OnMouseButtonPress(const MouseButtonEvent &e) {}

  virtual void OnMouseButtonRelease(const MouseButtonEvent &e) {}

  virtual void OnScroll(const ScrollEvent &e) {}

 protected:
  Entity *entity_ = nullptr;

  virtual void Start() {}

  virtual void Stop() {}
};


