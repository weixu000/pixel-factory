#include "PixelFactory/Image.h"

#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <stdexcept>

Image::Image(const std::string &image_path, int desired_channels) {
  data =
      stbi_load(image_path.c_str(), &width, &height, nullptr, desired_channels);
  if (data == nullptr) {
    throw std::runtime_error(std::string("stbi_load() failure: ") +
                             stbi_failure_reason());
  }
  channels = desired_channels;
  spdlog::debug("{} loaded", image_path);
}

Image::~Image() { stbi_image_free(data); }
