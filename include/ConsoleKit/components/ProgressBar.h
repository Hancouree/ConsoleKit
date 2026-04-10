#pragma once
#include "../IComponent.h"

namespace ck {
    class ProgressBar : public IComponent {
    public:
        ProgressBar(int finalValue);
        ProgressBar(int currentValue, int finalValue);

        void setWidth(int width);
        void setText(const std::string& str);
        std::string draw(const StyleContext& ctx = {}) const override;
        void update(int currentValue);
        void increment(int delta = 1);
        ProgressBar& withPercent(bool enable = true);
        ProgressBar& withSpeed(bool enable = true);
        ProgressBar& withETA(bool enable = true);
    private:
        int getPercent() const;
        double getSpeed() const;
        int getTimeLeft() const;
        std::string formatTime(int seconds) const;

        int m_minUpdateIntervalMs = 33;
        int m_width = 50;
        int m_finalValue;
        int m_currentValue = 0;
        std::string m_text;

        std::chrono::steady_clock::time_point m_startTime;
        std::chrono::steady_clock::time_point m_lastUpdate;

        bool m_showPercent;
        bool m_showSpeed;
        bool m_showETA;
    };
}