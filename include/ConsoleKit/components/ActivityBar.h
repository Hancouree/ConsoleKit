#pragma once
#include "../IComponent.h"

namespace ck {
    class ActivityBar : public IComponent {
    public:
        enum Style {
            Marquee,
            Pulse,
            Bounce,
        };

        ActivityBar();
        ActivityBar(const std::string& str);

        void setWidth(int width);
        void setStyle(Style s);
        void setText(const std::string& str);
        void setCurrentFrame(int frame);
        std::string draw(const StyleContext& ctx = {}) const override;
        void update();
    private:
        std::string drawMarquee() const;
        std::string drawPulse() const;
        std::string drawBounce() const;

        int m_minUpdateIntervalMs = 50;
        int m_width;
        Style m_style;
        int m_currentFrame;
        int m_delta;
        std::string m_text;
        std::chrono::steady_clock::time_point m_lastUpdate;
    };
}