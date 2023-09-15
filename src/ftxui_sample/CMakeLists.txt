
set(SOURCE main.cpp 
           headers.h
           casts.h
           Timer.h)

add_executable(intro ${SOURCE}
)

target_link_libraries(
  intro
  PRIVATE VulkanCm::VulkanCm_options
          VulkanCm::VulkanCm_warnings)

target_link_system_libraries(
  intro
  PRIVATE
          fmt::fmt
          spdlog::spdlog
          lefticus::tools)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
