#pragma once

class Entity;
class DrawOptions;

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

 protected:
  Entity *entity_ = nullptr;

  virtual void Start() {}

  virtual void Stop() {}
};


