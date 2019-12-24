#pragma once
#include <glm/glm.hpp>

#include <PixelFactory/components/Component.h>

class UpdateEvent;

class Camera : public Component {
 public:
  explicit Camera(float w, float h) { Resize(w, h); }

  void OnUpdate(const UpdateEvent &event);

  void Resize(float w, float h);

  [[nodiscard]] const glm::mat4 &Projection() const { return projection_; }

  [[nodiscard]] const glm::mat4 &View() const { return view_; }

  [[nodiscard]] const glm::vec3 &Eye() const { return eye_; }

  [[nodiscard]] float Width() const { return width_; }

  [[nodiscard]] float Height() const { return height_; }

 protected:
  void Start() override;

 private:
  glm::mat4 projection_, view_;
  glm::vec3 eye_;
  float width_, height_;
  float z_near_ = 0.1f, z_far_ = 1000.0f;
  float fovy_ = 60.f;
};


