#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <functional>

namespace ck {
    class ScreenManager {
    public:
        ScreenManager();
        ~ScreenManager();

        template <typename T>
        void add(T& component) {
            component.setScreenManager(this);

            m_components.push_back({
                [&component]() { return component.draw(); }
            });

            ++m_height;
        }

        void refresh();
        void print(const std::string& message);
    private:
        struct Component {
            std::function<std::string()> draw;
        };

        std::vector<Component> m_components;
        int m_height;
        std::deque<std::string> m_logs;
        bool m_finished;
    };

    class ProgressBar {
    public:
        ProgressBar(int finalValue);
        ProgressBar(int currentValue, int finalValue);

        void setWidth(int width);
        void setText(const std::string& str);
        void setScreenManager(ScreenManager* mgr);
        std::string draw();
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
        std::chrono::steady_clock::time_point m_lastDraw;

        bool m_showPercent;
        bool m_showSpeed;
        bool m_showETA;

        ScreenManager* m_mgr;
    };

    class Spinner {
    public:
        Spinner();
        Spinner(const std::string& str);

        void setText(const std::string& str);
        void setScreenManager(ScreenManager* mgr);
        std::string draw();
        void update();
    private:
        int m_minUpdateIntervalMs = 100;
        std::vector<std::string> m_frames;
        int m_currentFrame;
        std::string m_text;
        std::chrono::steady_clock::time_point m_lastUpdate;

        ScreenManager* m_mgr;
    };
}
