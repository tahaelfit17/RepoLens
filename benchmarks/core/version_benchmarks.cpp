#include "framework.hpp"

#include "repolens/core/version.hpp"

REPOLENS_BENCHMARK(core_version, 50, 1000) {
  const auto version = repolens::core::version();
  repolens::benchmark::do_not_optimize(version);
}
