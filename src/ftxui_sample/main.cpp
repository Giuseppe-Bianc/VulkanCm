#include "headers.h"
#include <internal_use_only/config.hpp>

int main() {
    try {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    const auto console = spdlog::stdout_color_mt(R"(console)");
    spdlog::set_default_logger(console);
    LINFO("info");
    } catch (const std::exception &e) {
      spdlog::error("Unhandled exception in main: {}", e.what());
    }
    SYSPAUSE();
    return EXIT_SUCCESS;
}
