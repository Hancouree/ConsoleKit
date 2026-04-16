#pragma once
#include <iostream>
#include <chrono>
#include <vector>

namespace ck {
    enum Color {
        Black = 30,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        Grey,
        DarkGrey = 90,
        LightRed,
        LightGreen,
        LightYellow,
        LightBlue,
        LightMagenta,
        LightCyan,
        White
    };

    struct StyleContext {
        Color fg = Grey;
        std::string apply() const;
    };

    namespace detail {
        std::chrono::steady_clock::time_point GET_NOW();
        std::vector<std::string> splitLines(const std::string& str);
        int visible_length(const std::string& s);

        inline constexpr const char* RESET = "\033[0m";
        inline constexpr const char* CLEAR_LINE = "\r\033[2K";
        inline constexpr const char* CURSOR_SAVE = "\033[s";
        inline constexpr const char* CURSOR_LOAD = "\033[u";
        inline constexpr const char* HIDE_CURSOR = "\033[?25l";
        inline constexpr const char* SHOW_CURSOR = "\033[?25h";

        std::string move_up(int n);
        std::string move_down(int n);
        std::string color_to_ansi(Color c);
    }
}