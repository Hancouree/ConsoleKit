#include "../../include/ConsoleKit/core/Common.h"
#include <regex>
#include <sstream>

namespace ck {
    namespace detail {
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

        int visible_length(const std::string& s)
        {
            static const std::regex ansi_re("\x1B\\[[0-9;]*[a-zA-Z]");
            std::string stripped = std::regex_replace(s, ansi_re, "");

            int len = 0;
            for (size_t i = 0; i < stripped.size(); ) {
                unsigned char c = stripped[i];
                if (c < 0x80) {
                    i += 1; 
                }
                else if (c < 0xE0) {
                    i += 2; 
                }
                else if (c < 0xF0) {
                    i += 3; 
                }
                else {
                    i += 4;
                }
                len++;
            }
            return len;
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
    }

    std::string StyleContext::apply() const
    {
        std::string res = detail::RESET;
        if (fg != Grey) {
            res += detail::color_to_ansi(fg);
        }

        return res;
    }
}