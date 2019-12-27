#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <PixelFactory/Window.h>
#include <PixelFactory/Application.h>
#include <PixelFactory/gl/GlContext.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/Time.h>
#include <PixelFactory/Event.h>

Window::Window(int width, int height, const std::string &title) {
  // Create the GLFW window.
  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  // Check if the window_ could not be created.
  if (!window_) {
    throw std::runtime_error("Failed to open GLFW window.");
  }

  glfwSetWindowUserPointer(window_, this);

  // Make the context of the window.
  glfwMakeContextCurrent(window_);
  context_ = std::make_unique<GlContext>();

  // Set swap interval to 1.
  glfwSwapInterval(0);

  SetupCallbacks();

  handler_ = std::make_unique<EventHandler>();

  Application::Instance().windows_.push_back(this);
}

Window::~Window() {
  glfwDestroyWindow(window_);
  Application::Instance().windows_.remove(this);
}

void Window::Loop() {
  glfwMakeContextCurrent(window_);
  handler_->ProcessEvent("Update", UpdateEvent{{}, Time::Delta()});
    Draw();
    glfwSwapBuffers(window_);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(window_);
}

Window *Window::Retrieve(GLFWwindow *w) {
  return reinterpret_cast<Window *>(glfwGetWindowUserPointer(w));
}

std::tuple<int, int> Window::FramebufferSize() const {
  int width, height;
  glfwGetFramebufferSize(window_, &width, &height);
  return std::make_tuple(width, height);
}

void Window::SetupCallbacks() {
// Set the key callback.
  glfwSetKeyCallback(window_,
                     [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                       KeyEvent e{{}, key, scancode, action, mods};
                       switch (e.action) {
                         case GLFW_PRESS:Retrieve(window)->handler_->ProcessEvent("KeyPress", e);
                           break;
                         case GLFW_RELEASE:Retrieve(window)->handler_->ProcessEvent("KeyRelease", e);
                           break;
                         default:break;
                       }
                     });
  // Set the window_ resize callback.
  glfwSetFramebufferSizeCallback(window_,
                                 [](GLFWwindow *window, int width, int height) {
                                   Retrieve(window)->handler_->ProcessEvent("Resize", ResizeEvent{{}, width, height});
                                 });
  // Set the mouse button callback.
  glfwSetMouseButtonCallback(window_,
                             [](GLFWwindow *window, int button, int action, int mods) {
                               double x, y;
                               glfwGetCursorPos(window, &x, &y);
                               MouseButtonEvent e{{}, float(x), float(y), button, action, mods};
                               switch (e.action) {
                                 case GLFW_PRESS:Retrieve(window)->handler_->ProcessEvent("MouseButtonPress", e);
                                   break;
                                 case GLFW_RELEASE:Retrieve(window)->handler_->ProcessEvent("MouseButtonRelease", e);
                                   break;
                                 default:break;
                               }
                             });
  // Set the cursor position callback.
  glfwSetCursorPosCallback(window_,
                           [](GLFWwindow *window, double x, double y) {
                             Retrieve(window)->handler_->ProcessEvent("MouseMove",
                                                                      CursorPositionEvent{{}, float(x), float(y)});
                           });

  // Set the scroll callback.
  glfwSetScrollCallback(window_,
                        [](GLFWwindow *window, double xoffset, double yoffset) {
                          Retrieve(window)->handler_->ProcessEvent("Scroll",
                                                                   ScrollEvent{{}, float(xoffset), float(yoffset)});
                        });
}
