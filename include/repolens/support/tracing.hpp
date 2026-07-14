#pragma once

#include "repolens/support/logging.hpp"

#include <chrono>
#include <string_view>

namespace repolens::support {

class TraceSpan final {
public:
  TraceSpan(
    const Logger& logger,
    std::string_view subsystem,
    std::string_view name,
    LogLevel level = LogLevel::trace) noexcept;
  TraceSpan(const TraceSpan&) = delete;
  TraceSpan& operator=(const TraceSpan&) = delete;
  TraceSpan(TraceSpan&&) = delete;
  TraceSpan& operator=(TraceSpan&&) = delete;
  ~TraceSpan() noexcept;

  void finish() noexcept;
  [[nodiscard]] bool finished() const noexcept;

private:
  const Logger* logger_;
  std::string_view subsystem_;
  std::string_view name_;
  LogLevel level_;
  std::chrono::steady_clock::time_point start_;
  bool finished_;
};

} // namespace repolens::support
