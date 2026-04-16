#include "../../include/ConsoleKit/components/ProgressBar.h"
#include "../../include/ConsoleKit/ScreenManager.h"
#include <string>

namespace ck {
    ProgressBar::ProgressBar(int currentValue, int finalValue, Container* parent)
        : StyledComponent(parent)
        , m_currentValue(currentValue)
        , m_finalValue(finalValue)
        , m_width(50)
        , m_startTime(detail::GET_NOW())
        , m_showPercent(false)
        , m_showSpeed(false)
        , m_showETA(false)
    {
        if (currentValue < 0 || currentValue > finalValue) {
            throw std::invalid_argument("currentValue out of range");
        }
    }

    void ProgressBar::setWidth(int width) {
        if (width <= 0) throw std::invalid_argument("Invalid width");
        m_width = width;
    }

    void ProgressBar::setText(const std::string& str) {
        m_text = str;
    }

    std::string ProgressBar::draw(const StyleContext& ctx) const {
        int filled = static_cast<double>(m_currentValue) / m_finalValue * m_width;

        std::string output;

        if (m_color != Grey) {
            output += detail::color_to_ansi(m_color);
        }

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

        output += ctx.apply();

        return output;
    }

    void ProgressBar::update(int currentValue) {
        m_currentValue = std::min(currentValue, m_finalValue);

        auto now = detail::GET_NOW();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastUpdate).count();

        if (elapsed > m_minUpdateIntervalMs || m_currentValue == m_finalValue) {
            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << detail::CLEAR_LINE << draw() << std::flush;
            }
            m_lastUpdate = now;
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
        return static_cast<int>(m_currentValue * 100.0 / m_finalValue);
    }

    double ProgressBar::getSpeed() const {
        auto now = detail::GET_NOW();
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
}