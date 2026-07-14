#include "repolens/support/logging.hpp"
#include "repolens/support/tracing.hpp"

#include "test_framework.hpp"

#include <sstream>
#include <string>
#include <string_view>

REPOLENS_TEST(log_level_names_are_stable) {
  REPOLENS_EXPECT_EQ(repolens::support::to_string(repolens::support::LogLevel::trace), "trace");
  REPOLENS_EXPECT_EQ(repolens::support::to_string(repolens::support::LogLevel::debug), "debug");
  REPOLENS_EXPECT_EQ(repolens::support::to_string(repolens::support::LogLevel::info), "info");
  REPOLENS_EXPECT_EQ(
    repolens::support::to_string(repolens::support::LogLevel::warning), "warning");
  REPOLENS_EXPECT_EQ(repolens::support::to_string(repolens::support::LogLevel::error), "error");
  REPOLENS_EXPECT_EQ(repolens::support::to_string(repolens::support::LogLevel::off), "off");
}

REPOLENS_TEST(log_level_parsing_accepts_stable_names) {
  REPOLENS_EXPECT_TRUE(
    repolens::support::log_level_from_string("trace") == repolens::support::LogLevel::trace);
  REPOLENS_EXPECT_TRUE(
    repolens::support::log_level_from_string("warn") == repolens::support::LogLevel::warning);
  REPOLENS_EXPECT_TRUE(!repolens::support::log_level_from_string("verbose").has_value());
}

REPOLENS_TEST(logger_filters_messages_below_minimum_level) {
  std::ostringstream output;
  repolens::support::StreamLogSink sink{output};
  repolens::support::Logger logger{sink, repolens::support::LogLevel::warning};

  logger.info("test", "hidden");
  logger.error("test", "visible");

  const auto text = output.str();
  REPOLENS_EXPECT_TRUE(text.find("hidden") == std::string::npos);
  REPOLENS_EXPECT_TRUE(text.find("[error] test: visible") != std::string::npos);
}

REPOLENS_TEST(trace_span_logs_elapsed_duration_when_enabled) {
  std::ostringstream output;
  repolens::support::StreamLogSink sink{output};
  repolens::support::Logger logger{sink, repolens::support::LogLevel::trace};

  {
    repolens::support::TraceSpan span{logger, "benchmark", "unit"};
    REPOLENS_EXPECT_TRUE(!span.finished());
  }

  const auto text = output.str();
  REPOLENS_EXPECT_TRUE(text.find("[trace] benchmark: unit completed in ") != std::string::npos);
}
