#include "PixelFactory/Application.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "PixelFactory/Time.h"
#include "PixelFactory/Window.h"

namespace {
void ErrorCallback(int error, const char *description) {
  // Print error.
  std::cerr << description << std::endl;
  exit(EXIT_FAILURE);
}
}  // namespace

Application::Application() {
  if (application_ != nullptr) {
    throw std::runtime_error("Only one Application instance allowed");
  }
  application_ = this;

  // Initialize GLFW.
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  // Setup error callback.
  glfwSetErrorCallback(ErrorCallback);
}

Application::~Application() {
  glfwTerminate();
  application_ = nullptr;
}

int Application::Run() {
  Time::Reset();
  bool all_closed;
  do {
    Time::Tick();
    glfwPollEvents();
    all_closed = true;
    for (auto &window : windows_) {
      if (!window->ShouldClose()) {
        window->Loop();
        all_closed = false;
      }
    }
  } while (!all_closed);

  return 0;
}
