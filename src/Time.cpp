#include <chrono>

#include <PixelFactory/Time.h>

float Time::now_ = 0.0f;
float Time::delta_ = 0.0f;

using namespace std::chrono;

void Time::Tick() {
  auto next = duration_cast<duration<float>>(steady_clock::now().time_since_epoch()).count();
  delta_ = next - now_;
  now_ = next;
}

void Time::Reset() {
  now_ = duration_cast<duration<float>>(steady_clock::now().time_since_epoch()).count();
}
