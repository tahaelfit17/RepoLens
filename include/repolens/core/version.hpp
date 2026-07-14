#pragma once

#include <string_view>

namespace repolens::core {

struct Version {
  int major;
  int minor;
  int patch;
  std::string_view text;
};

[[nodiscard]] std::string_view product_name() noexcept;
[[nodiscard]] Version version() noexcept;

} // namespace repolens::core
