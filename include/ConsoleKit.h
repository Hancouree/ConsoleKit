#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <chrono>

namespace ck {
    class ScreenManager;
    class ProgressBar;
    class Spinner;
    class ActivityBar;

    class IComponent {
    public:
        virtual ~IComponent() = default;
        virtual void setScreenManager(ScreenManager* mgr) = 0;
        virtual std::string draw() const = 0;
    };

    class ScreenManager {
    public:
        ScreenManager();
        ~ScreenManager();

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            static_assert(std::is_base_of_v<IComponent, T>, "T must inherit from IComponent");
            static_assert(std::is_constructible_v<T, Args...>, "T cannot be constructed with the given arguments");
            auto owner = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *owner;
            ref.setScreenManager(this);
            m_components.push_back(std::move(owner));
            ++m_height;
            return ref;
        };

        ProgressBar& addProgressBar(int finalValue);
        ProgressBar& addProgressBar(int currentValue, int finalValue);
        Spinner& addSpinner(const std::string& str = "");
        ActivityBar& addActivityBar(const std::string& str = "");

        void setMaxLogs(size_t n);
        void refresh();
        void log(const std::string& message);
    private:
        std::vector<std::unique_ptr<IComponent>> m_components;
        int m_height;
        std::deque<std::string> m_logs;
        size_t m_maxLogs;
        bool m_finished;
    };

    class ProgressBar : public IComponent {
    public:
        ProgressBar(int finalValue);
        ProgressBar(int currentValue, int finalValue);

        void setWidth(int width);
        void setText(const std::string& str);
        void setScreenManager(ScreenManager* mgr) override;
        std::string draw() const override;
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

    class Spinner : public IComponent {
    public:
        Spinner();
        Spinner(const std::string& str);

        void setText(const std::string& str);
        void setScreenManager(ScreenManager* mgr) override;
        void setFrames(const std::vector<std::string>& frames);
        std::string draw() const override;
        void update();
        void finish(const std::string& message = "Done");
    private:
        int m_minUpdateIntervalMs = 100;
        bool m_finished;
        std::vector<std::string> m_frames;
        int m_currentFrame;
        std::string m_text;
        std::chrono::steady_clock::time_point m_lastUpdate;

        ScreenManager* m_mgr;
    };

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
        void setScreenManager(ScreenManager* mgr) override;
        std::string draw() const override;
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

        ScreenManager* m_mgr;
    };
}
