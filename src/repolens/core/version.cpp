#include "repolens/core/version.hpp"

#include "repolens/build_config.hpp"

namespace repolens::core {

std::string_view product_name() noexcept {
  return "RepoLens";
}

Version version() noexcept {
  return Version{
    .major = REPOLENS_VERSION_MAJOR,
    .minor = REPOLENS_VERSION_MINOR,
    .patch = REPOLENS_VERSION_PATCH,
    .text = REPOLENS_VERSION_STRING,
  };
}

} // namespace repolens::core
