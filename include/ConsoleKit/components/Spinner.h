#pragma once
#include "../Component.h"

namespace ck {
    class Spinner : public StyledComponent {
    public:
        Spinner(const std::string& str = "", Container* parent = nullptr);

        void setText(const std::string& str);
        void setFrames(const std::vector<std::string>& frames);
        std::string draw(const StyleContext& ctx = {}) const override;
        void update();
        void finish(const std::string& message = "Done");
    private:
        int m_minUpdateIntervalMs = 100;
        bool m_finished;
        std::vector<std::string> m_frames;
        int m_currentFrame;
        std::string m_text;
        std::chrono::steady_clock::time_point m_lastUpdate;
    };
}