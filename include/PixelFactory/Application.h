#pragma once

#include <list>

class Window;

class Application {
 public:
  Application();

  ~Application();

  static Application &Instance() { return *application_; }

  int Run();

 private:
  static inline Application *application_ = nullptr;

  friend class Window;
  std::list<Window *> windows_;
};
