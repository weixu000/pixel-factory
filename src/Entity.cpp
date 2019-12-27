#include "PixelFactory/Entity.h"

Entity *Entity::AddChild(std::unique_ptr<Entity> child) {
  child->parent_ = this;
  child->world_transform_ =
      world_transform_.matrix * child->local_transform_.matrix;
  child->handler_ = handler_;
  children_.push_back(std::move(child));
  return children_.back().get();
}

void Entity::SetLocalTransform(const Transform &t) {
  local_transform_ = t;
  if (parent_) {
    world_transform_ =
        parent_->world_transform_.matrix * local_transform_.matrix;
  } else {
    world_transform_ = local_transform_;
  }
}
