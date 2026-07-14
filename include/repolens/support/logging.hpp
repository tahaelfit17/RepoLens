#pragma once

#include <chrono>
#include <iosfwd>
#include <mutex>
#include <optional>
#include <string_view>
#include <thread>

namespace repolens::support {

enum class LogLevel {
  trace = 0,
  debug = 1,
  info = 2,
  warning = 3,
  error = 4,
  off = 5,
};

struct LogRecord {
  LogLevel level;
  std::string_view subsystem;
  std::string_view message;
  std::chrono::system_clock::time_point timestamp;
  std::thread::id thread_id;
};

[[nodiscard]] std::string_view to_string(LogLevel level) noexcept;
[[nodiscard]] std::optional<LogLevel> log_level_from_string(std::string_view value) noexcept;
[[nodiscard]] bool should_log(LogLevel level, LogLevel minimum_level) noexcept;

class LogSink {
public:
  virtual ~LogSink() = default;
  virtual void write(const LogRecord& record) = 0;
};

class StreamLogSink final : public LogSink {
public:
  explicit StreamLogSink(std::ostream& output, LogLevel minimum_level = LogLevel::trace) noexcept;

  void set_minimum_level(LogLevel minimum_level) noexcept;
  [[nodiscard]] LogLevel minimum_level() const noexcept;

  void write(const LogRecord& record) override;

private:
  std::ostream* output_;
  LogLevel minimum_level_;
  std::mutex mutex_;
};

class Logger final {
public:
  explicit Logger(LogSink& sink, LogLevel minimum_level = LogLevel::info) noexcept;

  void set_minimum_level(LogLevel minimum_level) noexcept;
  [[nodiscard]] LogLevel minimum_level() const noexcept;
  [[nodiscard]] bool enabled(LogLevel level) const noexcept;

  void log(LogLevel level, std::string_view subsystem, std::string_view message) const;
  void trace(std::string_view subsystem, std::string_view message) const;
  void debug(std::string_view subsystem, std::string_view message) const;
  void info(std::string_view subsystem, std::string_view message) const;
  void warning(std::string_view subsystem, std::string_view message) const;
  void error(std::string_view subsystem, std::string_view message) const;

private:
  LogSink* sink_;
  LogLevel minimum_level_;
};

} // namespace repolens::support
