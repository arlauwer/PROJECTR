#pragma once
#include <format>
#include <print>
#include <string_view>

namespace Log {

namespace Color {
constexpr std::string_view Reset = "\033[0m";
constexpr std::string_view Bold  = "\033[1m";
constexpr std::string_view Dim   = "\033[2m";

// Foreground
constexpr std::string_view Red     = "\033[31m";
constexpr std::string_view Green   = "\033[32m";
constexpr std::string_view Yellow  = "\033[33m";
constexpr std::string_view Blue    = "\033[34m";
constexpr std::string_view Magenta = "\033[35m";
constexpr std::string_view Cyan    = "\033[36m";
constexpr std::string_view White   = "\033[37m";

// Bright foreground
constexpr std::string_view BrightRed     = "\033[91m";
constexpr std::string_view BrightGreen   = "\033[92m";
constexpr std::string_view BrightYellow  = "\033[93m";
constexpr std::string_view BrightBlue    = "\033[94m";
constexpr std::string_view BrightMagenta = "\033[95m";
constexpr std::string_view BrightCyan    = "\033[96m";
constexpr std::string_view BrightWhite   = "\033[97m";
}  // namespace Color

// ---- Core colored print ----

template <typename... Args> void print(std::string_view color, std::format_string<Args...> fmt, Args&&... args) {
    std::print("{}{}{}", color, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void println(std::string_view color, std::format_string<Args...> fmt, Args&&... args) {
    std::println("{}{}{}", color, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

// ---- Log levels ----

template <typename... Args> void info(std::format_string<Args...> fmt, Args&&... args) {
    std::print("{}[INFO]  {}", Color::BrightCyan, Color::Reset);
    std::println("{}{}{}{}", Color::White, Color::Bold, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void info_t(std::format_string<Args...> fmt, Args&&... args) {
    std::println("        {}{}{}", Color::White, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void warn(std::format_string<Args...> fmt, Args&&... args) {
    std::print("{}[WARN]  {}", Color::BrightYellow, Color::Reset);
    std::println("{}{}{}{}", Color::Yellow, Color::Bold, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void warn_t(std::format_string<Args...> fmt, Args&&... args) {
    std::println("        {}{}{}", Color::Yellow, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void error(std::format_string<Args...> fmt, Args&&... args) {
    std::print("{}[ERROR] {}", Color::BrightRed, Color::Reset);
    std::println("{}{}{}{}", Color::Red, Color::Bold, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void error_t(std::format_string<Args...> fmt, Args&&... args) {
    std::println("        {}{}{}", Color::Red, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void debug(std::format_string<Args...> fmt, Args&&... args) {
    std::print("{}[DEBUG] {}", Color::Blue, Color::Reset);
    std::println("{}{}{}{}", Color::Dim, Color::Bold, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

template <typename... Args> void debug_t(std::format_string<Args...> fmt, Args&&... args) {
    std::println("        {}{}{}", Color::Dim, std::format(fmt, std::forward<Args>(args)...), Color::Reset);
}

}  // namespace Log