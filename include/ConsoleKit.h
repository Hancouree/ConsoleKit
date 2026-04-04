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
    class Table;

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
        LightMagneta,
        LightCyan,
        White
    };

    class IComponent {
    public:
        virtual ~IComponent() = default;
        virtual void setScreenManager(ScreenManager* mgr) { m_mgr = mgr; }
        virtual void setColor(Color c) { m_color = c; }
        virtual Color getColor() const { return m_color; }
        virtual std::string draw() const = 0;
        virtual int getHeight() const { return 1; };
    protected:
        ScreenManager* m_mgr = nullptr;
        Color m_color = Grey;
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
            return ref;
        };

        ProgressBar& addProgressBar(int finalValue);
        ProgressBar& addProgressBar(int currentValue, int finalValue);
        Spinner& addSpinner(const std::string& str = "");
        ActivityBar& addActivityBar(const std::string& str = "");
        Table& addTable(const std::vector<std::string>& columns);

        void setMaxLogs(size_t n);
        void refresh();
        void log(const std::string& message);
    private:
        std::vector<std::unique_ptr<IComponent>> m_components;
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
    };

    class Spinner : public IComponent {
    public:
        Spinner();
        Spinner(const std::string& str);

        void setText(const std::string& str);
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
    };

    class Table : public IComponent {
    public:
        Table(const std::vector<std::string>& columns);

        void addRow(const std::vector<std::string>& row);
        std::string draw() const override;
        void update();

        std::vector<int> getColumnsWidth() const;
        int getHeight() const override;
    private:
        std::string pad(const std::string& str, int width) const;
        std::string drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const;
        std::string drawLine(const std::vector<int>& widths, char sep) const;

        int m_minUpdateIntervalMs = 50;
        std::vector<std::string> m_columns;
        std::vector<std::vector<std::string>> m_rows;
        bool m_wasInfoUpdated;
        std::chrono::steady_clock::time_point m_lastUpdate;
    };
}
