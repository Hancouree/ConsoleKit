#include "ConsoleKit.h"
#include <string>
#include <algorithm>

namespace ck {
    #define GET_NOW std::chrono::steady_clock::now()

    ScreenManager::ScreenManager() 
        : m_height(0)
        , m_finished(false)
    {
    }

    ScreenManager::~ScreenManager()
    {
        if (!m_finished) {
            int totalHeight = m_height + m_logs.size();
            if (totalHeight > 0) {
                std::cout << "\033[" << totalHeight << "B";
            }
            m_finished = true;
        }
    }

    void ScreenManager::refresh()
    {
        if (m_height == 0 && m_logs.size() == 0) return;

        std::cout << "\033[s";

        std::cout << "\033[" << m_height + m_logs.size() << "A";

        for (int i = 0; i < m_components.size(); ++i) {
            std::cout << "\r\033[K" << m_components[i].draw() << "\n";
        }

        for (const auto& msg : m_logs) {
            std::cout << "\r\033[K" << msg << "\n";
        }

        if (!m_components.empty() && m_logs.empty()) {
            std::cout << "\r\033[K";
        }

        std::cout << "\033[u" << std::flush;
    }

    void ScreenManager::print(const std::string& message)
    {
        m_logs.push_back(message);
        refresh();
    }

    ProgressBar::ProgressBar(int finalValue)
        : m_finalValue(finalValue)
        , m_width(50)
        , m_startTime(GET_NOW)
        , m_lastDraw(GET_NOW)
        , m_showPercent(false)
        , m_showSpeed(false)
        , m_showETA(false)
        , m_mgr(nullptr)
    {
    }

    ProgressBar::ProgressBar(int currentValue, int finalValue)
        : m_currentValue(currentValue)
        , m_finalValue(finalValue)
        , m_width(50)
        , m_startTime(GET_NOW)
        , m_lastDraw(GET_NOW)
        , m_showPercent(false)
        , m_showSpeed(false)
        , m_showETA(false)
        , m_mgr(nullptr)
    {
    }

    void ProgressBar::setWidth(int width) {
        m_width = width;
    }

    void ProgressBar::setText(const std::string& str) {
        m_text = str;
    }

    void ProgressBar::setScreenManager(ScreenManager* mgr)
    {
        m_mgr = mgr;
    }

    std::string ProgressBar::draw() {
        int filled = static_cast<double>(m_currentValue) / m_finalValue * m_width;

        std::string output;

        if (!m_text.empty()) {
            output += m_text + " ";
        }

        output += '[';

        for (int i = 0; i < m_width; ++i) {
            if (i < filled) {
                output += '=';
            }
            else if (i == filled) {
                output += '>';
            }
            else {
                output += ' ';
            }
        }

        output += ']';

        if (m_showPercent) {
            output += " " + std::to_string(getPercent()) + '%';
        }

        if (m_showSpeed) {
            output += " " + std::to_string(static_cast<int>(getSpeed())) + " its/s";
        }

        if (m_showETA) {
            output += " " + formatTime(getTimeLeft());
        }

        return output;
    }

    void ProgressBar::update(int currentValue) {
        m_currentValue = std::min(currentValue, m_finalValue);

        auto now = GET_NOW;
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastDraw).count();

        if (elapsed > m_minUpdateIntervalMs || m_currentValue == m_finalValue) {
            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << "\r" << draw() << std::flush;
            }
            m_lastDraw = now;
        }
    }

    void ProgressBar::increment(int delta) {
        update(m_currentValue + delta);
    }

    ProgressBar& ProgressBar::withPercent(bool enable) {
        m_showPercent = enable;
        return *this;
    }

    ProgressBar& ProgressBar::withSpeed(bool enable) {
        m_showSpeed = enable;
        return *this;
    }

    ProgressBar& ProgressBar::withETA(bool enable) {
        m_showETA = enable;
        return *this;
    }

    int ProgressBar::getPercent() const {
        return m_currentValue * 100 / m_finalValue;
    }

    double ProgressBar::getSpeed() const {
        auto now = GET_NOW;
        auto elapsed = std::chrono::duration<double>(now - m_startTime).count();
        if (elapsed < 0.001) return 0;
        return m_currentValue / elapsed;
    }

    int ProgressBar::getTimeLeft() const {
        auto speed = getSpeed();
        if (speed < 0.001) return 9999;
        int remaining = m_finalValue - m_currentValue;
        return static_cast<int>(remaining / speed);
    }

    std::string ProgressBar::formatTime(int seconds) const {
        if (seconds < 0) return "??:??";
        if (seconds < 60) return std::to_string(seconds) + "s";
        if (seconds < 3600) {
            int minutes = seconds / 60;
            int secs = seconds % 60;
            return std::to_string(minutes) + "m " + std::to_string(secs) + "s";
        }

        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        return std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    }

    Spinner::Spinner()
        : m_lastUpdate(GET_NOW)
        , m_currentFrame(0)
        , m_mgr(nullptr)
    {
        m_frames = {
            "|", "/", "-", "\\"
        };
    }

    Spinner::Spinner(const std::string& str) : Spinner() {
        m_text = str;
    }

    void Spinner::setText(const std::string& str) {
        m_text = str;
    }

    void Spinner::setScreenManager(ScreenManager* mgr)
    {
        m_mgr = mgr;
    }

    std::string Spinner::draw() {
        std::string output;

        if (!m_text.empty()) {
            output += m_text + " ";
        }

        output += m_frames[m_currentFrame];
        return output;
    }

    void Spinner::update() {
        auto now = GET_NOW;
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastUpdate).count();

        if (elapsed > m_minUpdateIntervalMs) {
            m_currentFrame = (m_currentFrame + 1) % m_frames.size();

            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << "\r" << draw() << std::flush;
            }

            m_lastUpdate = now;
        }
    }
}