include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(VulkanCm_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(VulkanCm_setup_options)
  option(VulkanCm_ENABLE_HARDENING "Enable hardening" ON)
  option(VulkanCm_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    VulkanCm_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    VulkanCm_ENABLE_HARDENING
    OFF)

  VulkanCm_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR VulkanCm_PACKAGING_MAINTAINER_MODE)
    option(VulkanCm_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(VulkanCm_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(VulkanCm_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(VulkanCm_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(VulkanCm_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(VulkanCm_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(VulkanCm_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(VulkanCm_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(VulkanCm_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(VulkanCm_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(VulkanCm_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(VulkanCm_ENABLE_PCH "Enable precompiled headers" OFF)
    option(VulkanCm_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(VulkanCm_ENABLE_IPO "Enable IPO/LTO" ON)
    option(VulkanCm_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(VulkanCm_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(VulkanCm_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(VulkanCm_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(VulkanCm_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(VulkanCm_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(VulkanCm_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(VulkanCm_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(VulkanCm_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(VulkanCm_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(VulkanCm_ENABLE_PCH "Enable precompiled headers" OFF)
    option(VulkanCm_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      VulkanCm_ENABLE_IPO
      VulkanCm_WARNINGS_AS_ERRORS
      VulkanCm_ENABLE_USER_LINKER
      VulkanCm_ENABLE_SANITIZER_ADDRESS
      VulkanCm_ENABLE_SANITIZER_LEAK
      VulkanCm_ENABLE_SANITIZER_UNDEFINED
      VulkanCm_ENABLE_SANITIZER_THREAD
      VulkanCm_ENABLE_SANITIZER_MEMORY
      VulkanCm_ENABLE_UNITY_BUILD
      VulkanCm_ENABLE_CLANG_TIDY
      VulkanCm_ENABLE_CPPCHECK
      VulkanCm_ENABLE_COVERAGE
      VulkanCm_ENABLE_PCH
      VulkanCm_ENABLE_CACHE)
  endif()

  VulkanCm_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (VulkanCm_ENABLE_SANITIZER_ADDRESS OR VulkanCm_ENABLE_SANITIZER_THREAD OR VulkanCm_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(VulkanCm_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(VulkanCm_global_options)
  if(VulkanCm_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    VulkanCm_enable_ipo()
  endif()

  VulkanCm_supports_sanitizers()

  if(VulkanCm_ENABLE_HARDENING AND VulkanCm_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR VulkanCm_ENABLE_SANITIZER_UNDEFINED
       OR VulkanCm_ENABLE_SANITIZER_ADDRESS
       OR VulkanCm_ENABLE_SANITIZER_THREAD
       OR VulkanCm_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${VulkanCm_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${VulkanCm_ENABLE_SANITIZER_UNDEFINED}")
    VulkanCm_enable_hardening(VulkanCm_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(VulkanCm_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(VulkanCm_warnings INTERFACE)
  add_library(VulkanCm_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  VulkanCm_set_project_warnings(
    VulkanCm_warnings
    ${VulkanCm_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(VulkanCm_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(VulkanCm_options)
  endif()

  include(cmake/Sanitizers.cmake)
  VulkanCm_enable_sanitizers(
    VulkanCm_options
    ${VulkanCm_ENABLE_SANITIZER_ADDRESS}
    ${VulkanCm_ENABLE_SANITIZER_LEAK}
    ${VulkanCm_ENABLE_SANITIZER_UNDEFINED}
    ${VulkanCm_ENABLE_SANITIZER_THREAD}
    ${VulkanCm_ENABLE_SANITIZER_MEMORY})

  set_target_properties(VulkanCm_options PROPERTIES UNITY_BUILD ${VulkanCm_ENABLE_UNITY_BUILD})

  if(VulkanCm_ENABLE_PCH)
    target_precompile_headers(
      VulkanCm_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(VulkanCm_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    VulkanCm_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(VulkanCm_ENABLE_CLANG_TIDY)
    VulkanCm_enable_clang_tidy(VulkanCm_options ${VulkanCm_WARNINGS_AS_ERRORS})
  endif()

  if(VulkanCm_ENABLE_CPPCHECK)
    VulkanCm_enable_cppcheck(${VulkanCm_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(VulkanCm_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    VulkanCm_enable_coverage(VulkanCm_options)
  endif()

  if(VulkanCm_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(VulkanCm_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(VulkanCm_ENABLE_HARDENING AND NOT VulkanCm_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR VulkanCm_ENABLE_SANITIZER_UNDEFINED
       OR VulkanCm_ENABLE_SANITIZER_ADDRESS
       OR VulkanCm_ENABLE_SANITIZER_THREAD
       OR VulkanCm_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    VulkanCm_enable_hardening(VulkanCm_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
