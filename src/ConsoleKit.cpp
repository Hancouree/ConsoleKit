#include "ConsoleKit.h"
#include <string>
#include <algorithm>
#include <numeric>

namespace ck {
    namespace {
        auto GET_NOW() {
            return std::chrono::steady_clock::now();
        }

        //SEQUENCES
        inline constexpr const char* RESET = "\033[0m";
        inline constexpr const char* CLEAR_LINE = "\r\033[2K";
        inline constexpr const char* CURSOR_SAVE = "\033[s";
        inline constexpr const char* CURSOR_LOAD = "\033[u";
        inline constexpr const char* HIDE_CURSOR = "\033[?25l";
        inline constexpr const char* SHOW_CURSOR = "\033[?25h";

        ////COLORS
        //inline constexpr const char* BLACK = "\033[30m";
        //inline constexpr const char* RED = "\033[31m";
        //inline constexpr const char* GREEN = "\033[32m";
        //inline constexpr const char* YELLOW = "\033[33m";
        //inline constexpr const char* BLUE = "\033[34m";
        //inline constexpr const char* MAGENTA = "\033[35m";
        //inline constexpr const char* CYAN = "\033[36m";
        //inline constexpr const char* WHITE = "\033[37m";

        ////BRIGHT COLORS
        //inline constexpr const char* BRIGHT_BLACK = "\033[90m";
        //inline constexpr const char* BRIGHT_RED = "\033[91m";
        //inline constexpr const char* BRIGHT_GREEN = "\033[92m";
        //inline constexpr const char* BRIGHT_YELLOW = "\033[93m";
        //inline constexpr const char* BRIGHT_BLUE = "\033[94m";
        //inline constexpr const char* BRIGHT_MAGENTA = "\033[95m";
        //inline constexpr const char* BRIGHT_CYAN = "\033[96m";
        //inline constexpr const char* BRIGHT_WHITE = "\033[97m";

        inline std::string move_up(int n) {
            return n > 0 ? "\033[" + std::to_string(n) + "A" : "";
        }

        inline std::string move_down(int n) {
            return n > 0 ? "\033[" + std::to_string(n) + "B" : "";
        }

        inline std::string color_to_ansi(Color c) {
            return "\033[" + std::to_string(c) + "m";
        }
    }

    ScreenManager::ScreenManager() 
        : m_finished(false)
        , m_maxLogs(25)
    {
        std::cout << HIDE_CURSOR;
    }

    ScreenManager::~ScreenManager()
    {
        if (!m_finished) {
            int m_height = std::accumulate(m_components.begin(), m_components.end(), 0,
                [](int x, const std::unique_ptr<IComponent>& c) {
                    return x + c->getHeight();
                });

            int totalHeight = m_height + m_logs.size();
            if (totalHeight > 0) {
                std::cout << move_down(totalHeight);
            }

            std::cout << SHOW_CURSOR;
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

    Table& ScreenManager::addTable(const std::vector<std::string>& columns)
    {
        return add<Table>(columns);
    }

    void ScreenManager::setMaxLogs(size_t n)
    {
        m_maxLogs = n;
    }

    void ScreenManager::refresh()
    {
        if (m_components.empty() && m_logs.empty()) return;

        int m_height = std::accumulate(m_components.begin(), m_components.end(), 0, 
            [](int x, const std::unique_ptr<IComponent>& c) {
                return x + c->getHeight();
        });

        std::cout << CURSOR_SAVE;

        std::cout << move_up(m_height + m_logs.size());

        for (size_t i = 0; i < m_components.size(); ++i) {
            std::string drawn = m_components[i]->draw();
            size_t pos = 0;
            size_t next;
            while ((next = drawn.find('\n', pos)) != std::string::npos) {
                std::cout << CLEAR_LINE << drawn.substr(pos, next - pos) << "\n";
                pos = next + 1;
            }

            std::cout << CLEAR_LINE << drawn.substr(pos) << "\n";
        }

        for (const auto& msg : m_logs) {
            std::cout << CLEAR_LINE << msg << "\n";
        }

        if (!m_components.empty() && m_logs.empty()) {
            std::cout << CLEAR_LINE;
        }

        std::cout << CURSOR_LOAD << std::flush;
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
                std::cout << CLEAR_LINE << draw() << std::flush;
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
                std::cout << CLEAR_LINE << draw() << std::flush;
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
            std::cout << CLEAR_LINE << draw() << "\n" << std::flush;
        }
    }

    ActivityBar::ActivityBar()
        : m_width(50)
        , m_currentFrame(0)
        , m_delta(1)
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
                std::cout << CLEAR_LINE << draw() << std::flush;
            }

            m_lastUpdate = now;
        }
    }

    std::string ActivityBar::drawMarquee() const
    {
        std::string output(m_width, ' ');
        output[m_currentFrame] = m_delta > 0 ? '>' : '<';
        return output;
    }

    std::string ActivityBar::drawPulse() const
    {
        return std::string(m_currentFrame, '=') + std::string(m_width - m_currentFrame, ' ');
    }

    std::string ActivityBar::drawBounce() const
    {
        std::string output(m_width, ' ');
        output[m_currentFrame] = 'O';
        return output;
    }

    Table::Table(const std::vector<std::string>& columns) 
        : m_wasInfoUpdated(false)
        , m_lastUpdate(GET_NOW())
    {
        m_columns = columns;
    }

    void Table::addRow(const std::vector<std::string>& row)
    {
        if (row.size() != m_columns.size()) {
            throw std::invalid_argument("Row size doesn't match column count");
        }
        m_rows.push_back(row);
        m_wasInfoUpdated = true;

        update();
    }

    std::string Table::draw() const
    {
        auto widths = getColumnsWidth();
        std::string output;

        output += drawLine(widths, '-') + '\n';
        output += drawRow(m_columns, widths) + '\n';
        output += drawLine(widths, '+') + '\n';
        for (const auto& row : m_rows)
            output += drawRow(row, widths) + '\n';
        if (m_rows.size() > 0)
            output += drawLine(widths, '-');

        return output;
    }

    void Table::update()
    {
        auto now = GET_NOW();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastUpdate).count();

        if (elapsed > m_minUpdateIntervalMs || m_wasInfoUpdated) {
            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << '\r' << draw() << std::flush;
            }

            m_lastUpdate = now;
            m_wasInfoUpdated = false;
        }
    }

    std::vector<int> Table::getColumnsWidth() const
    {
        std::vector<int> widths;
        for (size_t col = 0; col < m_columns.size(); ++col) {
            int w = m_columns[col].size();
            for (const auto& row : m_rows) {
                w = std::max(w, (int)row[col].size());
            }
            widths.push_back(w);
        }
        return widths;
    }

    int Table::getHeight() const
    {
        int rowsSize = m_rows.size();
        return rowsSize > 0 ? 4 + rowsSize : 3;
    }

    std::string Table::pad(const std::string& str, int width) const
    {
        return str + std::string(width - str.size(), ' ');
    }

    std::string Table::drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const
    {
        std::string output = "|";
        for (size_t i = 0; i < cells.size(); ++i) {
            output += pad(cells[i], widths[i]);
            output += (i + 1 < cells.size()) ? " | " : "|";
        }
        return output;
    }

    std::string Table::drawLine(const std::vector<int>& widths, char sep) const
    {
        std::string output = "+";  
        for (size_t i = 0; i < widths.size(); ++i) {
            output += std::string(widths[i], '-');
            output += (i + 1 < widths.size()) ? '-' + std::string(1, sep) + '-' : "+";  
        }
        return output;
    }
}