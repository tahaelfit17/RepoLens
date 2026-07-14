#include "repolens/support/logging.hpp"

#include <ostream>

namespace repolens::support {

std::string_view to_string(LogLevel level) noexcept {
  switch (level) {
  case LogLevel::trace:
    return "trace";
  case LogLevel::debug:
    return "debug";
  case LogLevel::info:
    return "info";
  case LogLevel::warning:
    return "warning";
  case LogLevel::error:
    return "error";
  case LogLevel::off:
    return "off";
  }

  return "unknown";
}

std::optional<LogLevel> log_level_from_string(std::string_view value) noexcept {
  if (value == "trace") {
    return LogLevel::trace;
  }
  if (value == "debug") {
    return LogLevel::debug;
  }
  if (value == "info") {
    return LogLevel::info;
  }
  if (value == "warning" || value == "warn") {
    return LogLevel::warning;
  }
  if (value == "error") {
    return LogLevel::error;
  }
  if (value == "off") {
    return LogLevel::off;
  }

  return std::nullopt;
}

bool should_log(LogLevel level, LogLevel minimum_level) noexcept {
  if (level == LogLevel::off || minimum_level == LogLevel::off) {
    return false;
  }

  return static_cast<int>(level) >= static_cast<int>(minimum_level);
}

StreamLogSink::StreamLogSink(std::ostream& output, LogLevel minimum_level) noexcept
  : output_{&output}, minimum_level_{minimum_level} {
}

void StreamLogSink::set_minimum_level(LogLevel minimum_level) noexcept {
  minimum_level_ = minimum_level;
}

LogLevel StreamLogSink::minimum_level() const noexcept {
  return minimum_level_;
}

void StreamLogSink::write(const LogRecord& record) {
  if (!should_log(record.level, minimum_level_)) {
    return;
  }

  const std::lock_guard lock{mutex_};
  *output_ << '[' << to_string(record.level) << "] " << record.subsystem << ": "
           << record.message << '\n';
}

Logger::Logger(LogSink& sink, LogLevel minimum_level) noexcept
  : sink_{&sink}, minimum_level_{minimum_level} {
}

void Logger::set_minimum_level(LogLevel minimum_level) noexcept {
  minimum_level_ = minimum_level;
}

LogLevel Logger::minimum_level() const noexcept {
  return minimum_level_;
}

bool Logger::enabled(LogLevel level) const noexcept {
  return should_log(level, minimum_level_);
}

void Logger::log(LogLevel level, std::string_view subsystem, std::string_view message) const {
  if (!enabled(level)) {
    return;
  }

  sink_->write(LogRecord{
    .level = level,
    .subsystem = subsystem,
    .message = message,
    .timestamp = std::chrono::system_clock::now(),
    .thread_id = std::this_thread::get_id(),
  });
}

void Logger::trace(std::string_view subsystem, std::string_view message) const {
  log(LogLevel::trace, subsystem, message);
}

void Logger::debug(std::string_view subsystem, std::string_view message) const {
  log(LogLevel::debug, subsystem, message);
}

void Logger::info(std::string_view subsystem, std::string_view message) const {
  log(LogLevel::info, subsystem, message);
}

void Logger::warning(std::string_view subsystem, std::string_view message) const {
  log(LogLevel::warning, subsystem, message);
}

void Logger::error(std::string_view subsystem, std::string_view message) const {
  log(LogLevel::error, subsystem, message);
}

} // namespace repolens::support
