#include "framework.hpp"

#include <cstdlib>

namespace {

struct Options {
  std::optional<std::size_t> samples;
  std::optional<std::size_t> operations;
  std::size_t warmup_samples = 3;
  std::string output_path;
  std::string baseline_path;
  double max_regression_percent = 10.0;
  bool help = false;
};

std::size_t parse_size(std::string_view value, std::string_view option_name) {
  std::size_t index = 0;
  const auto parsed = std::stoull(std::string{value}, &index);
  if (index != value.size() || parsed > std::numeric_limits<std::size_t>::max()) {
    throw std::runtime_error{"invalid value for " + std::string{option_name}};
  }
  return static_cast<std::size_t>(parsed);
}

double parse_percent(std::string_view value, std::string_view option_name) {
  std::size_t index = 0;
  const auto parsed = std::stod(std::string{value}, &index);
  if (index != value.size() || parsed < 0.0) {
    throw std::runtime_error{"invalid value for " + std::string{option_name}};
  }
  return parsed;
}

std::string_view require_value(int& index, int argc, char** argv, std::string_view option_name) {
  if (index + 1 >= argc) {
    throw std::runtime_error{"missing value for " + std::string{option_name}};
  }

  ++index;
  return argv[index];
}

Options parse_options(int argc, char** argv) {
  Options options;

  for (int index = 1; index < argc; ++index) {
    const std::string_view argument{argv[index]};

    if (argument == "--help" || argument == "-h") {
      options.help = true;
    } else if (argument == "--samples") {
      options.samples = parse_size(require_value(index, argc, argv, argument), argument);
    } else if (argument == "--operations") {
      options.operations = parse_size(require_value(index, argc, argv, argument), argument);
    } else if (argument == "--warmup") {
      options.warmup_samples = parse_size(require_value(index, argc, argv, argument), argument);
    } else if (argument == "--output") {
      options.output_path = std::string{require_value(index, argc, argv, argument)};
    } else if (argument == "--baseline") {
      options.baseline_path = std::string{require_value(index, argc, argv, argument)};
    } else if (argument == "--max-regression-percent") {
      options.max_regression_percent =
        parse_percent(require_value(index, argc, argv, argument), argument);
    } else {
      throw std::runtime_error{"unknown benchmark option '" + std::string{argument} + "'"};
    }
  }

  return options;
}

void print_help(std::ostream& output) {
  output << R"(RepoLens benchmarks

Usage:
  repolens_benchmarks [options]

Options:
  --samples N                  Override samples per benchmark.
  --operations N               Override operations per sample.
  --warmup N                   Warmup samples before measurement. Default: 3.
  --output PATH                Write benchmark results to PATH.
  --baseline PATH              Compare results with a previous benchmark output.
  --max-regression-percent N   Allowed median regression. Default: 10.
  -h, --help                   Show this help text.
)";
}

std::vector<repolens::benchmark::BenchmarkResult> run_all(const Options& options) {
  std::vector<repolens::benchmark::BenchmarkResult> results;
  results.reserve(repolens::benchmark::registry().size());

  for (const auto& benchmark : repolens::benchmark::registry()) {
    results.push_back(repolens::benchmark::run_benchmark(
      benchmark, options.samples, options.operations, options.warmup_samples));
  }

  return results;
}

} // namespace

int main(int argc, char** argv) {
  try {
    const auto options = parse_options(argc, argv);

    if (options.help) {
      print_help(std::cout);
      return EXIT_SUCCESS;
    }

    const auto results = run_all(options);
    repolens::benchmark::write_results(std::cout, results);

    if (!options.output_path.empty()) {
      std::ofstream output{options.output_path};
      if (!output) {
        std::cerr << "repolens_benchmarks: unable to open output file '" << options.output_path
                  << "'\n";
        return EXIT_FAILURE;
      }
      repolens::benchmark::write_results(output, results);
    }

    if (!options.baseline_path.empty()) {
      std::ifstream input{options.baseline_path};
      if (!input) {
        std::cerr << "repolens_benchmarks: unable to open baseline file '"
                  << options.baseline_path << "'\n";
        return EXIT_FAILURE;
      }
      const auto baseline = repolens::benchmark::read_baseline(input);
      if (repolens::benchmark::has_regressions(
            results, baseline, options.max_regression_percent, std::cerr)) {
        return EXIT_FAILURE;
      }
    }

    return EXIT_SUCCESS;
  } catch (const std::exception& error) {
    std::cerr << "repolens_benchmarks: " << error.what() << '\n';
    return EXIT_FAILURE;
  }
}
