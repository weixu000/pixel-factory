#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <functional>

class Event;

class EventHandler {
 public:
  using EventTag=std::string;

  void ProcessEvent(const EventTag &tag, const Event &event) {
    if (listeners_.count(tag)) {
      for (auto &func:listeners_[tag]) {
        func(event);
      }
    }
  }

  template<typename EventType, typename Func>
  void Bind(const EventTag &tag, Func func) {
    if (!listeners_.count(tag)) {
      listeners_[tag] = std::list<std::function<void(const Event &)>>();
    }
    listeners_[tag].push_back([func](const Event &event) {
      func(*static_cast<const EventType *>(&event));
    });
  }

 private:
  std::unordered_map<EventTag, std::list<std::function<void(const Event &)>>> listeners_;
};


