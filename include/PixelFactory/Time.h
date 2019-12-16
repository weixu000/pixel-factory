#pragma once

class Time {
 public:
  Time() = delete;

  static void Reset();

  static void Tick();

  static float Now() { return now_; }

  static float Delta() { return delta_; }

 private:
  static float now_;
  static float delta_;
};


