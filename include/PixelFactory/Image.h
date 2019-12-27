#pragma once

#include <string>

struct Image {
  Image(const std::string &image_path, int desired_channels);

  ~Image();

  int width = 0, height = 0, channels = 0;
  unsigned char *data;
};


