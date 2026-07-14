#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace repolens::testing {

struct TestCase {
  std::string_view name;
  void (*run)();
};

inline std::vector<TestCase>& registry() {
  static std::vector<TestCase> tests;
  return tests;
}

struct Registrar {
  Registrar(std::string_view name, void (*run)()) {
    registry().push_back(TestCase{.name = name, .run = run});
  }
};

inline void fail(std::string_view file, int line, std::string_view message) {
  std::ostringstream stream;
  stream << file << ':' << line << ": " << message;
  throw std::runtime_error{stream.str()};
}

template <typename Left, typename Right>
void expect_equal(
  const Left& left,
  const Right& right,
  std::string_view left_expression,
  std::string_view right_expression,
  std::string_view file,
  int line) {
  if (!(left == right)) {
    std::ostringstream stream;
    stream << "expected " << left_expression << " == " << right_expression << ", got " << left
           << " and " << right;
    fail(file, line, stream.str());
  }
}

inline int run_all() {
  int failures = 0;

  for (const auto& test : registry()) {
    try {
      test.run();
      std::cout << "[PASS] " << test.name << '\n';
    } catch (const std::exception& error) {
      ++failures;
      std::cerr << "[FAIL] " << test.name << '\n';
      std::cerr << "       " << error.what() << '\n';
    } catch (...) {
      ++failures;
      std::cerr << "[FAIL] " << test.name << '\n';
      std::cerr << "       unknown exception\n";
    }
  }

  if (failures != 0) {
    std::cerr << failures << " test(s) failed\n";
    return 1;
  }

  std::cout << registry().size() << " test(s) passed\n";
  return 0;
}

} // namespace repolens::testing

#define REPOLENS_TEST(test_name)                                                                  \
  void test_name();                                                                               \
  static const ::repolens::testing::Registrar test_name##_registrar{#test_name, &test_name};      \
  void test_name()

#define REPOLENS_EXPECT_TRUE(condition)                                                           \
  do {                                                                                             \
    if (!(condition)) {                                                                            \
      ::repolens::testing::fail(__FILE__, __LINE__, "expected true: " #condition);                \
    }                                                                                              \
  } while (false)

#define REPOLENS_EXPECT_EQ(left, right)                                                            \
  ::repolens::testing::expect_equal((left), (right), #left, #right, __FILE__, __LINE__)
