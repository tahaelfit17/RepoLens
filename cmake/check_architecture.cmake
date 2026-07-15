if(NOT DEFINED REPOLENS_SOURCE_DIR)
  message(FATAL_ERROR "REPOLENS_SOURCE_DIR is required")
endif()

cmake_path(ABSOLUTE_PATH REPOLENS_SOURCE_DIR NORMALIZE OUTPUT_VARIABLE repo_root)

set(source_globs
  "${repo_root}/apps/*.cpp"
  "${repo_root}/apps/*.hpp"
  "${repo_root}/apps/**/*.cpp"
  "${repo_root}/apps/**/*.hpp"
  "${repo_root}/include/**/*.hpp"
  "${repo_root}/src/**/*.cpp"
  "${repo_root}/src/**/*.hpp")

file(GLOB_RECURSE source_files ${source_globs})

set(violations "")

function(repolens_check_file relative_path absolute_path)
  file(READ "${absolute_path}" content)

  set(forbidden_pattern "")
  set(forbidden_reason "")

  if(relative_path MATCHES "^(include|src)/repolens/support/")
    set(forbidden_pattern "#[ \t]*include[ \t]*[<\"]repolens/(core|application|infrastructure|adapters|cli|apps)/")
    set(forbidden_reason "support is the lowest shared layer and must not depend on higher layers")
  elseif(relative_path MATCHES "^(include|src)/repolens/core/")
    set(forbidden_pattern "#[ \t]*include[ \t]*[<\"]repolens/(application|infrastructure|adapters|cli|apps)/")
    set(forbidden_reason "core must not depend on application, infrastructure, adapters, or CLI")
  elseif(relative_path MATCHES "^(include|src)/repolens/application/")
    set(forbidden_pattern "#[ \t]*include[ \t]*[<\"]repolens/(infrastructure|adapters|cli|apps)/")
    set(forbidden_reason "application use cases must not depend on infrastructure, adapters, or CLI")
  elseif(relative_path MATCHES "^(include|src)/repolens/infrastructure/")
    set(forbidden_pattern "#[ \t]*include[ \t]*[<\"]repolens/(cli|apps)/")
    set(forbidden_reason "infrastructure must not depend on CLI entry points")
  endif()

  if(NOT forbidden_pattern STREQUAL "")
    string(REGEX MATCH "${forbidden_pattern}" layer_match "${content}")
    if(layer_match)
      set(violations
        "${violations}\n${relative_path}: ${forbidden_reason}; matched '${layer_match}'"
        PARENT_SCOPE)
    endif()
  endif()

  if(relative_path MATCHES "^(include|src)/repolens/core/")
    set(core_io_pattern "#[ \t]*include[ \t]*[<\"](filesystem|fstream|iostream|cstdio|cstdlib)[>\"]")
    string(REGEX MATCH "${core_io_pattern}" core_io_match "${content}")
    if(core_io_match)
      set(violations
        "${violations}\n${relative_path}: core must not perform direct IO or process interaction; matched '${core_io_match}'"
        PARENT_SCOPE)
    endif()
  endif()
endfunction()

foreach(source_file IN LISTS source_files)
  file(RELATIVE_PATH relative_path "${repo_root}" "${source_file}")
  string(REPLACE "\\" "/" relative_path "${relative_path}")
  repolens_check_file("${relative_path}" "${source_file}")
endforeach()

if(NOT violations STREQUAL "")
  message(FATAL_ERROR "RepoLens architecture boundary violations:${violations}")
endif()

message(STATUS "RepoLens architecture boundaries passed")
