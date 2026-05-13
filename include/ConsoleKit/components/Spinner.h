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
        void setUpdateInterval(int ms);
        void setPosition(Position pos);
        void finish(const std::string& message = "Done");

        std::string draw(const StyleContext& ctx = {}) const override;
        void tick() override;
    private:
        int m_intervalMs = 100;
        int m_currentFrame = 0;
        bool m_finished = false;
        Position m_position = Position::Right;
        std::string m_text;
        std::vector<std::string> m_frames;
        std::chrono::steady_clock::time_point m_lastTick;
	};
}

