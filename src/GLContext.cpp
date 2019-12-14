#include <iostream>
#include <stdexcept>

#include <PixelFactory/GLContext.h>

GLContext::GLContext(int width, int height, const std::string &title) {
  // 4x antialiasing.
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Create the GLFW window_.
  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  // Check if the window_ could not be created.
  if (!window_) {
    throw std::runtime_error("Failed to open GLFW window.");
  }

  glfwSetWindowUserPointer(window_, this);

  // Make the context of the window.
  glfwMakeContextCurrent(window_);
  if (!gladLoadGL()) {
    throw std::runtime_error("Failed to initialize GLAD.");
  }
  // Get info of GPU and supported OpenGL version.
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl
            << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl
            << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  // Set swap interval to 1.
  glfwSwapInterval(0);

  SetupCallbacks();
}

void GLContext::SetupCallbacks() {
  // Set the key callback.
  glfwSetKeyCallback(window_,
                     [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                       Retrieve(window)->KeyCallback(key, scancode, action, mods);
                     });
  // Set the window_ resize callback.
  glfwSetWindowSizeCallback(window_,
                            [](GLFWwindow *window, int width, int height) {
                              Retrieve(window)->ResizeCallback(width, height);
                            });
  // Set the mouse button callback.
  glfwSetMouseButtonCallback(window_,
                             [](GLFWwindow *window, int button, int action, int mods) {
                               Retrieve(window)->MouseButtonCallback(button, action, mods);
                             });
  // Set the cursor position callback.
  glfwSetCursorPosCallback(window_,
                           [](GLFWwindow *window, double x, double y) {
                             Retrieve(window)->CursorPosCallback(x, y);
                           });

  // Set the scroll callback.
  glfwSetScrollCallback(window_,
                        [](GLFWwindow *window, double xoffset, double yoffset) {
                          Retrieve(window)->ScrollCallback(xoffset, yoffset);
                        });
}
void GLContext::Loop() {
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();
    Update();
    Draw();
    glfwSwapBuffers(window_);
  }
}
