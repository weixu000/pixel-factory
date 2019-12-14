#include <iostream>

#include <PixelFactory/GLContext.h>

class Window : public GLContext {
 public:
  Window() : GLContext(800, 600, "Empty Window") {}

  void Draw() override {
    glClear(GL_COLOR_BUFFER_BIT);
  }
};

void ErrorCallback(int error, const char *description) {
  // Print error.
  std::cerr << description << std::endl;
  exit(EXIT_FAILURE);
}

void SetupGLFW() {
  // Initialize GLFW.
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Setup error callback.
  glfwSetErrorCallback(ErrorCallback);
}

int main() {
  SetupGLFW();

  Window window;
  window.Loop();
}


