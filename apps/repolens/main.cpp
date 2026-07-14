#include "repolens/core/version.hpp"

#include <iostream>
#include <string_view>

namespace {

constexpr std::string_view help_text = R"(RepoLens

Usage:
  repolens --help
  repolens --version

RepoLens publishes repo-owned repository intelligence for humans and AI tools.

Options:
  -h, --help      Show this help text.
  -V, --version   Show the RepoLens version.
)";

void print_version(std::ostream& output) {
  const auto version = repolens::core::version();
  output << "repolens " << version.text << '\n';
}

void print_help(std::ostream& output) {
  output << help_text;
}

} // namespace

int main(int argc, char** argv) {
  if (argc == 1) {
    print_help(std::cout);
    return 0;
  }

  const std::string_view first_argument{argv[1]};

  if (first_argument == "--help" || first_argument == "-h") {
    print_help(std::cout);
    return 0;
  }

  if (first_argument == "--version" || first_argument == "-V") {
    print_version(std::cout);
    return 0;
  }

  std::cerr << "repolens: unknown argument '" << first_argument << "'\n";
  std::cerr << "Try 'repolens --help'.\n";
  return 2;
}
