#pragma once
#include <string>
#include <vector>
#include "../core/Component.h"

namespace ck {
	class Spinner final : public StyledComponent
	{
    public:
        enum class Position { Left, Right };

        Spinner(const std::string& text = "", Container* parent = nullptr);

        void setText(const std::string& text);
        void setFrames(const std::vector<std::string>& frames);
        void setPosition(Position pos);
        void finish(const std::string& message = "Done");

        std::string draw(const StyleContext& ctx = {}) const override;
        void tick() override;
    private:
        void setTheme(Theme theme) override {};

        int m_currentFrame;
        bool m_finished;
        Position m_position;
        std::string m_text;
        std::vector<std::string> m_frames;

        static constexpr const char* SPINNER_PIPE = "|";
        static constexpr const char* SPINNER_SLASH = "/";
        static constexpr const char* SPINNER_DASH = "-";
        static constexpr const char* SPINNER_BACKSLASH = "\\";
	};
}

