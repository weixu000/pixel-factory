#pragma once

#include <memory>
#include <list>

#include <PixelFactory/Transform.h>
#include <PixelFactory/components/Component.h>

class ResizeEvent;
class KeyEvent;
class MouseButtonEvent;
class CursorPositionEvent;
class ScrollEvent;
class DrawOptions;
class EventHandler;

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

  template<typename T>
  T *AddComponent(std::unique_ptr<T> component) {
    component->Attach(this);
    components_.push_back(std::move(component));
    return static_cast<T *>(components_.back().get());
  }

  template <typename T>
  std::remove_reference_t<T> *AddComponent(T &&component) {
    return AddComponent(std::make_unique<std::remove_reference_t<T>>(
        std::forward<T>(component)));
  }

  [[nodiscard]] EventHandler *GetEventHandler() const { return handler_; }

  void SetEventHandler(EventHandler *handler) { handler_ = handler; }

  template <typename Func> void ForEachDescendant(Func func) const {
    func(*this);
    ForEachChild(func);
  }

  template <typename Func> void ForEachComponent(Func func) const {
    for (auto &component : components_) {
      func(*component);
    }
  }

private:
  Entity *parent_ = nullptr;

  std::list<std::unique_ptr<Entity>> children_;

  Transform local_transform_;
  Transform world_transform_;

  std::list<std::unique_ptr<Component>> components_;

  template <typename Func> void ForEachChild(Func func) const {
    for (auto &child : children_) {
      func(*child);
    }
  }

  EventHandler *handler_ = nullptr;
};


