#include <iostream>
#include <stdexcept>

#include <PixelFactory/GL/GLContext.h>
#include <PixelFactory/Event.h>
#include <PixelFactory/EventHandler.h>
#include <PixelFactory/Time.h>

GLContext::GLContext(int width, int height, const std::string &title) {
  // Create the GLFW window.
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  handler_ = std::make_unique<EventHandler>();
}

void GLContext::SetupCallbacks() {
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
void GLContext::Loop() {
  Time::Reset();
  while (!glfwWindowShouldClose(window_)) {
    Time::Tick();
    glfwPollEvents();
    handler_->ProcessEvent("Update", UpdateEvent{{}, Time::Delta()});
    Draw();
    glfwSwapBuffers(window_);
  }
}
