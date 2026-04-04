#include "ConsoleKit.h"
#include <string>
#include <algorithm>

namespace ck {
    namespace {
        auto GET_NOW() {
            return std::chrono::steady_clock::now();
        }
    }

    ScreenManager::ScreenManager() 
        : m_height(0)
        , m_finished(false)
        , m_maxLogs(25)
    {
        std::cout << "\033[?25l";
    }

    ScreenManager::~ScreenManager()
    {
        if (!m_finished) {
            int totalHeight = m_height + m_logs.size();
            if (totalHeight > 0) {
                std::cout << "\033[" << totalHeight << "B";
            }
            std::cout << "\033[?25h";
            m_finished = true;
        }
    }

    ProgressBar& ScreenManager::addProgressBar(int finalValue)
    {
        return add<ProgressBar>(finalValue);
    }

    ProgressBar& ScreenManager::addProgressBar(int currentValue, int finalValue)
    {
        return add<ProgressBar>(currentValue, finalValue);
    }

    Spinner& ScreenManager::addSpinner(const std::string& str)
    {
        return add<Spinner>(str);
    }

    ActivityBar& ScreenManager::addActivityBar(const std::string& str)
    {
        return add<ActivityBar>(str);
    }

    void ScreenManager::setMaxLogs(size_t n)
    {
        m_maxLogs = n;
    }

    void ScreenManager::refresh()
    {
        if (m_height == 0 && m_logs.size() == 0) return;

        std::cout << "\033[s";

        std::cout << "\033[" << m_height + m_logs.size() << "A";

        for (size_t i = 0; i < m_components.size(); ++i) {
            std::cout << "\r\033[K" << m_components[i]->draw() << "\n";
        }

        for (const auto& msg : m_logs) {
            std::cout << "\r\033[K" << msg << "\n";
        }

        if (!m_components.empty() && m_logs.empty()) {
            std::cout << "\r\033[K";
        }

        std::cout << "\033[u" << std::flush;
    }

    void ScreenManager::log(const std::string& message)
    {
        if (m_logs.size() >= m_maxLogs) {
            m_logs.pop_front();
        }

        m_logs.push_back(message);
        refresh();
    }

    ProgressBar::ProgressBar(int finalValue)
        : m_currentValue(0)
        , m_finalValue(finalValue)
        , m_width(50)
        , m_startTime(GET_NOW())
        , m_lastDraw(m_startTime)
        , m_showPercent(false)
        , m_showSpeed(false)
        , m_showETA(false)
        , m_mgr(nullptr)
    {
        if (finalValue <= 0) throw std::invalid_argument("finalValue must be > 0");
    }

    ProgressBar::ProgressBar(int currentValue, int finalValue) : ProgressBar(finalValue)
    {
        if (currentValue < 0 || currentValue > finalValue) {
            throw std::invalid_argument("currentValue out of range");
        }

        m_currentValue = currentValue;
    }

    void ProgressBar::setWidth(int width) {
        if (width <= 0) throw std::invalid_argument("Invalid width");
        m_width = width;
    }

    void ProgressBar::setText(const std::string& str) {
        m_text = str;
    }

    void ProgressBar::setScreenManager(ScreenManager* mgr)
    {
        m_mgr = mgr;
    }

    std::string ProgressBar::draw() const {
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

        auto now = GET_NOW();
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
        return static_cast<int>(m_currentValue * 100.0 / m_finalValue);
    }

    double ProgressBar::getSpeed() const {
        auto now = GET_NOW();
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
        : m_lastUpdate(GET_NOW())
        , m_currentFrame(0)
        , m_finished(false)
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

    void Spinner::setFrames(const std::vector<std::string>& frames)
    {
        if (frames.empty()) throw std::invalid_argument("frames can not be empty");
        m_frames = frames;
    }

    std::string Spinner::draw() const {
        if (m_finished) {
            return m_text;
        }

        std::string output;

        if (!m_text.empty()) {
            output += m_text + " ";
        }

        output += m_frames[m_currentFrame];
        return output;
    }

    void Spinner::update() {
        if (m_finished) return; 

        auto now = GET_NOW();
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

    void Spinner::finish(const std::string& message)
    {
        m_finished = true;
        m_text = message;

        if (m_mgr) {
            m_mgr->refresh();
        }
        else {
            std::cout << "\r\033[K" << draw() << "\n" << std::flush;
        }
    }

    ActivityBar::ActivityBar()
        : m_width(50)
        , m_currentFrame(0)
        , m_delta(1)
        , m_mgr(nullptr)
        , m_lastUpdate(GET_NOW())
        , m_style(Style::Marquee)
    {
    }

    ActivityBar::ActivityBar(const std::string& str) : ActivityBar()
    {
        m_text = str;
    }

    void ActivityBar::setWidth(int width)
    {
        if (width <= 0) throw std::invalid_argument("Invalid width");
        m_width = width;
    }

    void ActivityBar::setStyle(Style s)
    {
        m_style = s;
    }

    void ActivityBar::setText(const std::string& str)
    {
        m_text = str;
    }

    void ActivityBar::setCurrentFrame(int frame)
    {
        if (frame < 0 || frame > m_width) throw std::invalid_argument("Invalid frame");
        m_currentFrame = frame;
    }

    void ActivityBar::setScreenManager(ScreenManager* mgr)
    {
        m_mgr = mgr;
    }

    std::string ActivityBar::draw() const
    {
        std::string output;

        output += '[';

        switch (m_style)
        {
        case ck::ActivityBar::Marquee:
            output += drawMarquee();
            break;
        case ck::ActivityBar::Pulse:
            output += drawPulse();
            break;
        case ck::ActivityBar::Bounce:
            output += drawBounce();
            break;
        }

        output += ']';

        if (!m_text.empty()) {
            output += " " + m_text;
        }

        return output;
    }

    void ActivityBar::update()
    {
        auto now = GET_NOW();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastUpdate).count();

        if (elapsed > m_minUpdateIntervalMs) {
            m_currentFrame += m_delta;
            if (m_currentFrame >= m_width || m_currentFrame < 0) {
                m_delta *= -1;
                m_currentFrame += m_delta * 2;
            }

            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << "\r" << draw() << std::flush;
            }

            m_lastUpdate = now;
        }
    }

    std::string ActivityBar::drawMarquee() const
    {
        std::string output;
        for (int i = 0; i < m_width; ++i) {
            if (i < m_currentFrame) output += ' ';
            else if (i == m_currentFrame) output += (m_delta == 1 ? '>' : '<');
            else output += ' ';
        }
        return output;
    }

    std::string ActivityBar::drawPulse() const
    {
        std::string output;
        for (int i = 0; i < m_width; ++i) {
            if (i <= m_currentFrame) output += '=';
            else output += ' ';
        }
        return output;
    }

    std::string ActivityBar::drawBounce() const
    {
        std::string output;
        for (int i = 0; i < m_width; ++i) {
            if (i < m_currentFrame) output += ' ';
            else if (i == m_currentFrame) output += 'O';
            else output += ' ';
        }
        return output;
    }
}