cmake_minimum_required(VERSION 3.25)

project(
  RepoLens
  VERSION 0.1.0
  DESCRIPTION "Repo-owned repository intelligence for humans and AI tools"
  LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)

option(REPOLENS_BUILD_TESTS "Build RepoLens tests" ${PROJECT_IS_TOP_LEVEL})
option(REPOLENS_BUILD_BENCHMARKS "Build RepoLens benchmarks" ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(
  cmake/repolens_config.hpp.in
  "${CMAKE_CURRENT_BINARY_DIR}/generated/repolens/build_config.hpp"
  @ONLY)

function(repolens_target_warnings target_name)
  if(MSVC)
    target_compile_options(${target_name} PRIVATE /W4 /permissive- /EHsc)
  else()
    target_compile_options(
      ${target_name}
      PRIVATE -Wall
              -Wextra
              -Wpedantic
              -Wconversion
              -Wsign-conversion)
  endif()
endfunction()

add_library(repolens_support INTERFACE)
add_library(RepoLens::support ALIAS repolens_support)

target_compile_features(repolens_support INTERFACE cxx_std_23)
target_include_directories(
  repolens_support
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_library(repolens_core STATIC
  src/repolens/core/version.cpp)
add_library(RepoLens::core ALIAS repolens_core)

target_compile_features(repolens_core PUBLIC cxx_std_23)
target_include_directories(
  repolens_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_link_libraries(repolens_core PUBLIC RepoLens::support)
repolens_target_warnings(repolens_core)

add_executable(repolens
  apps/repolens/main.cpp)
target_compile_features(repolens PRIVATE cxx_std_23)
target_link_libraries(repolens PRIVATE RepoLens::core)
repolens_target_warnings(repolens)

if(BUILD_TESTING AND REPOLENS_BUILD_TESTS)
  add_executable(repolens_tests
    tests/core/version_tests.cpp
    tests/main.cpp)
  target_compile_features(repolens_tests PRIVATE cxx_std_23)
  target_link_libraries(repolens_tests PRIVATE RepoLens::core)
  target_include_directories(repolens_tests PRIVATE tests)
  repolens_target_warnings(repolens_tests)

  add_test(NAME repolens.core COMMAND repolens_tests)
  add_test(
    NAME repolens.architecture
    COMMAND
      "${CMAKE_COMMAND}"
      "-DREPOLENS_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_architecture.cmake")
endif()

install(TARGETS repolens RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
