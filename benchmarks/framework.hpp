#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace repolens::benchmark {

struct BenchmarkCase {
  std::string_view name;
  std::size_t samples;
  std::size_t operations_per_sample;
  void (*run)();
};

struct BenchmarkResult {
  std::string name;
  std::size_t samples;
  std::size_t operations_per_sample;
  double min_ns;
  double median_ns;
  double mean_ns;
  double max_ns;
};

inline std::vector<BenchmarkCase>& registry() {
  static std::vector<BenchmarkCase> benchmarks;
  return benchmarks;
}

struct Registrar {
  explicit Registrar(BenchmarkCase benchmark) {
    registry().push_back(benchmark);
  }
};

template <typename Value>
void do_not_optimize(const Value& value) {
#if defined(__GNUC__) || defined(__clang__)
  asm volatile("" : : "g"(&value) : "memory");
#elif defined(_MSC_VER)
  (void)&value;
  _ReadWriteBarrier();
#else
  const volatile auto* volatile sink = &value;
  (void)sink;
#endif
}

inline BenchmarkResult run_benchmark(
  const BenchmarkCase& benchmark,
  std::optional<std::size_t> sample_override,
  std::optional<std::size_t> operation_override,
  std::size_t warmup_samples) {
  const auto samples = sample_override.value_or(benchmark.samples);
  const auto operations_per_sample = operation_override.value_or(benchmark.operations_per_sample);

  if (samples == 0) {
    throw std::runtime_error{"benchmark samples must be greater than zero"};
  }

  if (operations_per_sample == 0) {
    throw std::runtime_error{"benchmark operations per sample must be greater than zero"};
  }

  for (std::size_t sample = 0; sample < warmup_samples; ++sample) {
    for (std::size_t operation = 0; operation < operations_per_sample; ++operation) {
      benchmark.run();
    }
  }

  std::vector<double> durations;
  durations.reserve(samples);

  for (std::size_t sample = 0; sample < samples; ++sample) {
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t operation = 0; operation < operations_per_sample; ++operation) {
      benchmark.run();
    }
    const auto stop = std::chrono::steady_clock::now();
    const auto elapsed =
      std::chrono::duration<double, std::nano>{stop - start}.count() /
      static_cast<double>(operations_per_sample);
    durations.push_back(elapsed);
  }

  std::ranges::sort(durations);

  double total = 0.0;
  for (const auto duration : durations) {
    total += duration;
  }

  return BenchmarkResult{
    .name = std::string{benchmark.name},
    .samples = samples,
    .operations_per_sample = operations_per_sample,
    .min_ns = durations.front(),
    .median_ns = durations[durations.size() / 2],
    .mean_ns = total / static_cast<double>(durations.size()),
    .max_ns = durations.back(),
  };
}

inline void write_results(std::ostream& output, const std::vector<BenchmarkResult>& results) {
  output << "# repolens-benchmark-v1\n";
  output << "# name\tsamples\toperations_per_sample\tmin_ns\tmedian_ns\tmean_ns\tmax_ns\n";
  output << std::fixed << std::setprecision(3);

  for (const auto& result : results) {
    output << result.name << '\t' << result.samples << '\t' << result.operations_per_sample << '\t'
           << result.min_ns << '\t' << result.median_ns << '\t' << result.mean_ns << '\t'
           << result.max_ns << '\n';
  }
}

inline std::vector<std::string> split_tab_separated(std::string_view line) {
  std::vector<std::string> fields;
  std::size_t start = 0;

  while (start <= line.size()) {
    const auto separator = line.find('\t', start);
    if (separator == std::string_view::npos) {
      fields.emplace_back(line.substr(start));
      break;
    }
    fields.emplace_back(line.substr(start, separator - start));
    start = separator + 1;
  }

  return fields;
}

inline std::map<std::string, BenchmarkResult> read_baseline(std::istream& input) {
  std::map<std::string, BenchmarkResult> baseline;
  std::string line;
  std::size_t line_number = 0;

  while (std::getline(input, line)) {
    ++line_number;

    if (line.empty() || line.starts_with('#')) {
      continue;
    }

    const auto fields = split_tab_separated(line);
    if (fields.size() != 7) {
      std::ostringstream message;
      message << "invalid benchmark baseline line " << line_number << ": expected 7 fields";
      throw std::runtime_error{message.str()};
    }

    baseline.emplace(
      fields[0],
      BenchmarkResult{
        .name = fields[0],
        .samples = static_cast<std::size_t>(std::stoull(fields[1])),
        .operations_per_sample = static_cast<std::size_t>(std::stoull(fields[2])),
        .min_ns = std::stod(fields[3]),
        .median_ns = std::stod(fields[4]),
        .mean_ns = std::stod(fields[5]),
        .max_ns = std::stod(fields[6]),
      });
  }

  return baseline;
}

inline bool has_regressions(
  const std::vector<BenchmarkResult>& current,
  const std::map<std::string, BenchmarkResult>& baseline,
  double max_regression_percent,
  std::ostream& error_output) {
  bool failed = false;
  const auto allowed_multiplier = 1.0 + (max_regression_percent / 100.0);

  for (const auto& result : current) {
    const auto existing = baseline.find(result.name);
    if (existing == baseline.end()) {
      continue;
    }

    const auto allowed_median = existing->second.median_ns * allowed_multiplier;
    if (result.median_ns > allowed_median) {
      failed = true;
      error_output << "benchmark regression: " << result.name << " median "
                   << result.median_ns << " ns exceeded allowed " << allowed_median
                   << " ns from baseline " << existing->second.median_ns << " ns\n";
    }
  }

  return failed;
}

} // namespace repolens::benchmark

#define REPOLENS_BENCHMARK(benchmark_name, sample_count, operation_count)                         \
  void benchmark_name();                                                                          \
  static const ::repolens::benchmark::Registrar benchmark_name##_registrar{                       \
    ::repolens::benchmark::BenchmarkCase{                                                         \
      .name = #benchmark_name,                                                                    \
      .samples = sample_count,                                                                    \
      .operations_per_sample = operation_count,                                                   \
      .run = &benchmark_name}};                                                                   \
  void benchmark_name()
