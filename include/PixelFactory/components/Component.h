#pragma once

class Entity;
class DrawOptions;

class Component {
 public:
  void Attach(Entity *entity) {
    entity_ = entity;
    Start();
  }

  [[nodiscard]] Entity *GetEntity() const { return entity_; }

 protected:
  Entity *entity_ = nullptr;

  virtual void Start() {}
};
