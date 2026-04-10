#include "../../include/ConsoleKit/components/ActivityBar.h"
#include "../../include/ConsoleKit/ScreenManager.h"

namespace ck {
    ActivityBar::ActivityBar()
        : m_width(50)
        , m_currentFrame(0)
        , m_delta(1)
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

    std::string ActivityBar::draw(const StyleContext& ctx) const
    {
        std::string output;

        if (m_color != Grey) {
            output += color_to_ansi(m_color);
        }

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

        output += ctx.apply();

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
}