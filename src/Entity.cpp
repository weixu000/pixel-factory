#include <PixelFactory/Entity.h>
#include <PixelFactory/Event.h>

Entity *Entity::AddChild(std::unique_ptr<Entity> child) {
  child->parent_ = this;
  child->world_transform_ = world_transform_.matrix * child->local_transform_.matrix;
  children_.push_back(std::move(child));
  return children_.back().get();
}

void Entity::SetLocalTransform(const Transform &t) {
  local_transform_ = t;
  if (parent_) {
    world_transform_ = parent_->world_transform_.matrix * local_transform_.matrix;
  } else {
    world_transform_ = local_transform_;
  }
}

void Entity::Draw(const DrawOptions &options) {
  ForEachComponent([&](Component &component) { component.Draw(options); });
  ForEachChild([&](Entity &child) { child.Draw(options); });
}

void Entity::Update() {
  ForEachComponent([&](Component &component) { component.Update(); });
  ForEachChild([&](Entity &child) { child.Update(); });
}

void Entity::OnKeyPress(const KeyEvent &e) {
  ForEachComponent([&](Component &component) { component.OnKeyPress(e); });
  ForEachChild([&](Entity &child) { child.OnKeyPress(e); });
}

void Entity::OnKeyRelease(const KeyEvent &e) {
  ForEachComponent([&](Component &component) { component.OnKeyRelease(e); });
  ForEachChild([&](Entity &child) { child.OnKeyRelease(e); });
}

void Entity::OnMouseMove(const CursorPositionEvent &e) {
  ForEachComponent([&](Component &component) { component.OnMouseMove(e); });
  ForEachChild([&](Entity &child) { child.OnMouseMove(e); });
}

void Entity::OnMouseButtonPress(const MouseButtonEvent &e) {
  ForEachComponent([&](Component &component) { component.OnMouseButtonPress(e); });
  ForEachChild([&](Entity &child) { child.OnMouseButtonPress(e); });
}

void Entity::OnMouseButtonRelease(const MouseButtonEvent &e) {
  ForEachComponent([&](Component &component) { component.OnMouseButtonRelease(e); });
  ForEachChild([&](Entity &child) { child.OnMouseButtonRelease(e); });
}

void Entity::OnScroll(const ScrollEvent &e) {
  ForEachComponent([&](Component &component) { component.OnScroll(e); });
  ForEachChild([&](Entity &child) { child.OnScroll(e); });
}
