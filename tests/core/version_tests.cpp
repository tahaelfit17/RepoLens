#include "repolens/core/version.hpp"

#include "test_framework.hpp"

#include <string_view>

REPOLENS_TEST(version_matches_project_metadata) {
  const auto version = repolens::core::version();

  REPOLENS_EXPECT_EQ(version.major, 0);
  REPOLENS_EXPECT_EQ(version.minor, 1);
  REPOLENS_EXPECT_EQ(version.patch, 0);
  REPOLENS_EXPECT_EQ(version.text, std::string_view{"0.1.0"});
}

REPOLENS_TEST(product_name_is_stable) {
  REPOLENS_EXPECT_EQ(repolens::core::product_name(), std::string_view{"RepoLens"});
}
