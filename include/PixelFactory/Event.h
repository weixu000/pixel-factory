#pragma once

struct Event {

};

struct ResizeEvent : public Event {
  int width, height;
};

struct KeyEvent : public Event {
  int key;
  int scancode;
  int action;
  int mods;
};

struct MouseButtonEvent : public Event {
  float x;
  float y;
  int button;
  int action;
  int mods;
};

struct CursorPositionEvent : public Event {
  float x;
  float y;
};

struct ScrollEvent : public Event {
  float x_offset, y_offset;
};

