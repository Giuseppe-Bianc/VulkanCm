#pragma once
#include "headers.h"

class Timer {
public:
#pragma optimize("gt", on)
    inline Timer() noexcept : start_time(std::chrono::high_resolution_clock::now()) {}
#pragma optimize("gt", on)
    inline void stop() noexcept {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

#pragma optimize("gt", on)
    inline long double elapsedNanoseconds() const noexcept {
        const auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration<long double, std::nano>(end - start_time).count();
    }
#pragma optimize("gt", on)
    inline long double elapsedMicroseconds() const noexcept { return elapsedNanoseconds() / C_LD(1000.0); }

#pragma optimize("gt", on)
    inline long double elapsedMilliseconds() const noexcept { return elapsedNanoseconds() / C_LD(1'000'000.0); }
#pragma optimize("gt", on)
    inline long double elapsedSeconds() const noexcept { return elapsedNanoseconds() / 1'000'000'000.0; }
#pragma optimize("gt", on)
    inline void elapsedNsToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} ns)", msg, elapsedNanoseconds());
    }
#pragma optimize("gt", on)
    inline void elapsedMcsToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} us)", msg, elapsedMicroseconds());
    }
#pragma optimize("gt", on)
    inline void elapsedMllisToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} ms)", msg, elapsedMilliseconds());
    }
#pragma optimize("gt", on)
    inline void elapsedSecondsToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} s)", msg, elapsedSeconds());
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running = true;
};
