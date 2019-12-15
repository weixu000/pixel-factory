#pragma once

#include <memory>
#include <list>

#include <PixelFactory/Transform.h>
#include <PixelFactory/Component.h>

class ResizeEvent;
class KeyEvent;
class MouseButtonEvent;
class CursorPositionEvent;
class ScrollEvent;
class DrawOptions;

class Component;

class Entity {
 public:
  explicit Entity(const glm::mat4 &t = glm::mat4(1.0f)) : local_transform_(t) {}

  [[nodiscard]] Entity *Parent() const { return parent_; }

  Entity *AddChild(std::unique_ptr<Entity> child);

  Entity *AddChild(Entity &&child) {
    return AddChild(std::make_unique<Entity>(std::move(child)));
  }

  [[nodiscard]] const Transform &LocalTransform() const { return local_transform_; }

  [[nodiscard]] const Transform &WorldTransform() const { return world_transform_; }

  void SetLocalTransform(const Transform &t);

  void Draw(const DrawOptions &options);

  void Update();

  template<typename T>
  T *AddComponent(std::unique_ptr<T> component) {
    component->Attach(this);
    components_.push_back(std::move(component));
    return static_cast<T *>(components_.back().get());
  }

  template<typename T>
  std::remove_reference_t<T> *AddComponent(T &&component) {
    return AddComponent(std::make_unique<std::remove_reference_t<T>>(std::forward<T>(component)));
  }

  void OnKeyPress(const KeyEvent &e);

  void OnKeyRelease(const KeyEvent &e);

  void OnMouseMove(const CursorPositionEvent &e);

  void OnMouseButtonPress(const MouseButtonEvent &e);

  void OnMouseButtonRelease(const MouseButtonEvent &e);

  void OnScroll(const ScrollEvent &e);

 private:
  Entity *parent_ = nullptr;

  std::list<std::unique_ptr<Entity>> children_;

  Transform local_transform_;
  Transform world_transform_;

  std::list<std::unique_ptr<Component>> components_;

  template<typename Func>
  void ForEachChild(Func func) {
    for (auto &child:children_) {
      func(*child);
    }
  }

  template<typename Func>
  void ForEachComponent(Func func) {
    for (auto &component:components_) {
      func(*component);
    }
  }
};


