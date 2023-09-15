#pragma once
#define GLFW_PLATFORM_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_WIN32
#define GLM_FORCE_SIZE_T_LENGTH
#define GLM_FORCE_PURE
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_FORCE_CXX20
#define GLM_FORCE_UNRESTRICTED_GENTYPE
#define GLM_FORCE_PRECISION_HIGHP_DOUBLE
#define GLM_FORCE_PRECISION_HIGHP_INT
#define GLM_FORCE_PRECISION_HIGHP_FLOAT
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(push)
#pragma warning(disable : 6386 6385 4005 26481 4459)
#endif
// clang-format off
//#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>
#include <cassert>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execution>
#ifdef _WIN32 // Check if the target platform is Windows
  #ifdef _MSC_VER // Check if the compiler is MSVC
    #include <format>
  #endif
#endif
#include <fstream>
//#include <glm/glm.hpp>
//#include <glm/gtc/constants.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtc/random.hpp>
//#include <glm/gtx/norm.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <numbers>
#include <omp.h>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <spdlog/cfg/env.h>
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <source_location>
#include <type_traits>

// clang-format on
#include "casts.h"
// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(pop)
#endif

#pragma optimize("gt", on)
[[nodiscard]] static constexpr auto calcolaCentro(const int &width, const int &w) noexcept { return (width - w) / 2; }

#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define CALC_CENTRO(width, w) calcolaCentro(width, w)
template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::vec<L, T, Q> &vector) {
    return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::mat<C, R, T, Q> &matrix) {
    return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q> inline OStream &operator<<(OStream &os, glm::qua<T, Q> quaternion) {
    return os << glm::to_string(quaternion);
}
#define GLWFERR(error, description) LERROR("GLFW Error ({0}): {1}", error, description);
#define SYSPAUSE()                                                                                                               \
    LINFO("Press enter to exit...");                                                                                             \
    std::cin.ignore();
#define PRINTVER(version)                                                                                                        \
    LINFO("System can support vulkan Variant: {}, Major: {}, Minor: {}", VK_API_VERSION_VARIANT(version),                        \
          VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version));

#define VK_CHECK(f)                                                                                                              \
    do {                                                                                                                         \
        VkResult res = (f);                                                                                                      \
        if(res != VK_SUCCESS) [[unlikely]] {                                                                                     \
            auto loc = std::source_location::current();                                                                          \
            LCRITICAL("Fatal : VkResult is \"{0}\" from {1} in {2} at line {3}", #f, string_VkResult(res), loc.file_name(),      \
                      loc.line());                                                                                               \
            assert(res == VK_SUCCESS);                                                                                           \
        }                                                                                                                        \
    } while(0)

#define VK_CHECK_SWAPCHAIN(f)                                                                                                    \
    do {                                                                                                                         \
        VkResult res = (f);                                                                                                      \
        if(res != VK_SUCCESS || res != VK_SUBOPTIMAL_KHR || res != VK_ERROR_OUT_OF_DATE_KHR) [[unlikely]] {                      \
            constexpr auto loc = std::source_location::current();                                                                \
            LCRITICAL("Fatal : VkResult is \"{0}\" from{1} in {2} at line {3}", #f, string_VkResult(res), loc.file_name(),       \
                      loc.line());                                                                                               \
            assert(result_ == VK_SUCCESS || result_ == VK_SUBOPTIMAL_KHR || result_ == VK_ERROR_OUT_OF_DATE_KHR);                \
        }                                                                                                                        \
    } while(0)

/*
#pragma optimize("gt", on)
    [[nodiscard]] inline static std::string_view debugCallbackString(
        const VkDebugUtilsMessageTypeFlagsEXT &messageType) noexcept {
        switch(messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            return "Validation layer [General]: ";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            return "Validation layer [Validation]: ";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            return "Validation layer [Performance]: ";
        [[unlikely]] default:
            return "Validation layer: ";
        }
    }
#pragma optimize("gt", on)
    inline static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                               VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                               [[maybe_unused]] void *pUserData) {
        const std::string_view prefix = debugCallbackString(messageType);
        const std::string_view msg = pCallbackData->pMessage;
        switch(messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            LTRACE("{0}{1}", prefix, msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LINFO("{0}{1}", prefix, msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LWARN("{0}{1}", prefix, msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LERROR("{0}{1}", prefix, msg);
            break;
        [[unlikely]] default:
            LDEBUG("{0}{1}", prefix, msg);
            break;
        }
        return VK_FALSE;
    }*/

template <typename EnumType> static constexpr std::string VulkanEnumToString(EnumType value) {
    static_assert(std::is_enum_v<EnumType>, "EnumType deve essere un tipo di enumerazione.");

    // Utilizza una serie di if constexpr per selezionare la funzione appropriata da vk_enum_string_helper.h
    if constexpr(std::is_same_v<EnumType, VkResult>) {
        LINFO("called");
        return string_VkResult(value);
    } else if constexpr(std::is_same_v<EnumType, VkFormat>) {
        return string_VkFormat(value);
    } else if constexpr(std::is_same_v<EnumType, VkStructureType>) {
        return strig_VkStructureType(value);
    } else if constexpr(std::is_same_v<EnumType, VkPipelineCacheHeaderVersion>) {
        return string_VkPipelineCacheHeaderVersion(value);
    } else if constexpr(std::is_same_v<EnumType, VkImageLayout>) {
        return string_VkImageLayout(value);
    } else if constexpr(std::is_same_v<EnumType, VkObjectType>) {
        return string_VkObjectType(value);
    } else if constexpr(std::is_same_v<EnumType, VkVendorId>) {
        return string_VkVendorId(value);
    } else if constexpr(std::is_same_v<EnumType, VkInternalAllocationType>) {
        return string_VkInternalAllocationType(value);
    } else if constexpr(std::is_same_v<EnumType, VkImageTiling>) {
        return string_VkImageTiling(value);
    }
    LINFO("type ={}", typeid(EnumType).name());
    throw std::invalid_argument("Tipo di enumerazione non supportato.");
}
#define VKENUM_TO_STRING(x) VulkanEnumToString<decltype(x)>(x)
#define VKTRACE_ENUM(x) SPDLOG_TRACE("result = {}", VKENUM_TO_STRING(x))
#define VKDEBUG_ENUM(x) SPDLOG_DEBUG("result = {}", VKENUM_TO_STRING(x))
#define VKINFO_ENUM(x) SPDLOG_INFO("result = {}", VKENUM_TO_STRING(x))
#define VKWARN_ENUM(x) SPDLOG_WARN("result = {}", VKENUM_TO_STRING(x))
#define VKERROR_ENUM(x) SPDLOG_ERROR("result = {}", VKENUM_TO_STRING(x))
#define VKCRITICAL_ENUM(x) SPDLOG_CRITICAL("result = {}", VKENUM_TO_STRING(x))
static inline constexpr long double infinity = std::numeric_limits<long double>::infinity();
static inline constexpr long double pi = std::numbers::pi_v<long double>;
static inline constexpr long double twoPi = 2 * pi;
static inline constexpr long double halfPi = pi / 2;
// Dimensioni della finestra
static inline constexpr double ratioW = 16.0;
static inline constexpr double ratioH = 9.0;
static inline constexpr double aspect_ratio = ratioW / ratioH;
static inline constexpr int imageF = 70;
static inline constexpr int w = C_I(ratioW * imageF);
static inline constexpr int h = C_I(ratioH * imageF);
static inline constexpr std::size_t ST_w = C_ST(w);
static inline constexpr std::size_t ST_h = C_ST(h);
static inline constexpr double scale = 256.0;
static inline constexpr double invStHMinusOne = 1.0 / C_D(ST_h - 1);
static inline constexpr double invStWMinusOne = 1.0 / C_D(ST_w - 1);
static inline constexpr unsigned long long doublesize = sizeof(double);
static inline constexpr std::string_view windowTitle = "Vulkan window";


/*
 Timer t;
if(!glfwInit()) [[unlikely]] {
    LCRITICAL("Failed to initialize GLFW.");
    throw VKRAppError("Failed to initialize GLFW");
}
if(!glfwVulkanSupported()) {
    glfwTerminate();
    LCRITICAL("Failed to initialize GLFW. vulkan not supported");
    throw VKRAppError("Failed to initialize GLFW. vulkan not supported");
}
glfwSetErrorCallback(errorCallback);
glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
t.stop();
const auto glfwsetuptime = t.elapsedMilliseconds();
Timer tt;
window = glfwCreateWindow(this->mWidth, this->mHeight, this->mWindowName.data(), nullptr, nullptr);
if(window == nullptr) [[unlikely]] {
    LCRITICAL("Failed to create GLFW window.");
    glfwTerminate();
    throw VKRAppError("Failed to create GLFW window");
}
tt.stop();
glfwSetWindowUserPointer(window, this);
glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
const auto wcreationtime = tt.elapsedMilliseconds();
Timer ttt;
glfwSetKeyCallback(window, keyCallback);
// Ottenimento delle dimensioni del monitor primario
GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
if(primaryMonitor == nullptr) [[unlikely]] {
    LCRITICAL("Failed to get the primary monitor.");
    glfwDestroyWindow(window);
    glfwTerminate();
    throw VKRAppError("Failed to get primary monitor");
}
const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
if(mode == nullptr) [[unlikely]] {
    LCRITICAL("Failed to get the video mode of the primary monitor.");
    glfwDestroyWindow(window);
    glfwTerminate();
    throw VKRAppError("Failed to get video mode");
}
const int monitorWidth = mode->width;
const int monitorHeight = mode->height;

// Calcolo delle coordinate per centrare la finestra
int windowWidth;
int windowHeight;
glfwGetWindowSize(window, &windowWidth, &windowHeight);
const int centerX = CALC_CENTRO(monitorWidth, windowWidth);
const int centerY = CALC_CENTRO(monitorHeight, windowHeight);

// Posizionamento della finestra al centro del monitor
glfwSetWindowPos(window, centerX, centerY);

int posX = 0;
int posY = 0;
glfwGetWindowPos(window, &posX, &posY);
if(posX != centerX || posY != centerY) [[unlikely]] {
    LCRITICAL("Failed to position the window at the center.");
    glfwDestroyWindow(window);
    glfwTerminate();
    throw VKRAppError("Failed to set window position centered");
}
ttt.stop();
const auto wrepostime = ttt.elapsedMilliseconds();

glfwShowWindow(window);
const auto totalsetuptime = glfwsetuptime + wcreationtime + wrepostime;
LINFO("setupp time for glfw = {0} ms", glfwsetuptime);
LINFO("time for creating the window {0}: (w: {1}, h: {2}, pos:(x:{3}, y:{4})) = {5} ms", windowTitle.data(), w, h, centerX,
      centerY, wcreationtime);
LINFO("time for repositionig the window{0} = {1} ms", windowTitle.data(), wrepostime);
LINFO("toltal setupp time = {0} ms", totalsetuptime);*/