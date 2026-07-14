#include "repolens/support/tracing.hpp"

#include <iomanip>
#include <sstream>

namespace repolens::support {

TraceSpan::TraceSpan(
  const Logger& logger,
  std::string_view subsystem,
  std::string_view name,
  LogLevel level) noexcept
  : logger_{&logger},
    subsystem_{subsystem},
    name_{name},
    level_{level},
    start_{std::chrono::steady_clock::now()},
    finished_{false} {
}

TraceSpan::~TraceSpan() noexcept {
  finish();
}

void TraceSpan::finish() noexcept {
  if (finished_) {
    return;
  }

  finished_ = true;

  if (logger_ == nullptr || !logger_->enabled(level_)) {
    return;
  }

  try {
    const auto stop = std::chrono::steady_clock::now();
    const auto elapsed_ms = std::chrono::duration<double, std::milli>{stop - start_}.count();

    std::ostringstream message;
    message << name_ << " completed in " << std::fixed << std::setprecision(3) << elapsed_ms
            << " ms";
    logger_->log(level_, subsystem_, message.str());
  } catch (...) {
  }
}

bool TraceSpan::finished() const noexcept {
  return finished_;
}

} // namespace repolens::support
