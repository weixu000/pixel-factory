#pragma once

struct ResizeEvent {
  int width, height;
};

struct KeyEvent {
  int key;
  int scancode;
  int action;
  int mods;
};

struct MouseButtonEvent {
  float x;
  float y;
  int button;
  int action;
  int mods;
};

struct CursorPositionEvent {
  float x;
  float y;
};

struct ScrollEvent {
  float x_offset, y_offset;
};

