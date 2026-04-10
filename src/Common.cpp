#include "../include/ConsoleKit/Common.h"
#include <regex>

namespace ck {
    std::chrono::steady_clock::time_point GET_NOW()
    {
        return std::chrono::steady_clock::now();
    }

    std::vector<std::string> splitLines(const std::string& str)
    {
        std::vector<std::string> lines;
        std::stringstream ss(str);
        std::string line;
        while (std::getline(ss, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    size_t visible_length(const std::string& s)
    {
        static const std::regex ansi_re("\x1B\\[[0-9;]*[a-zA-Z]");
        return std::regex_replace(s, ansi_re, "").length();
    }

    std::string move_up(int n)
    {
        return n > 0 ? "\033[" + std::to_string(n) + "A" : "";
    }

    std::string move_down(int n)
    {
        return n > 0 ? "\033[" + std::to_string(n) + "B" : "";
    }

    std::string color_to_ansi(Color c)
    {
        return "\033[" + std::to_string(c) + "m";
    }

    std::string StyleContext::apply() const
    {
        std::string res = RESET;
        if (fg != Grey) {
            res += color_to_ansi(fg);
        }

        return res;
    }
}