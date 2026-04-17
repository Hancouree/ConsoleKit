#include "../../include/ConsoleKit/components/Spinner.h"
#include "../../include/ConsoleKit/ScreenManager.h"

namespace ck {
    Spinner::Spinner(const std::string& str, Container* parent)
        : StyledComponent(parent)
        , m_currentFrame(0)
        , m_finished(false)
        , m_text(str)
        , m_spinnerPosition(SpinnerPosition::Right)
    {
        m_frames = {
            "|", "/", "-", "\\"
        };
    }

    void Spinner::setText(const std::string& str) {
        m_text = str;
    }

    void Spinner::setFrames(const std::vector<std::string>& frames)
    {
        if (frames.empty()) throw std::invalid_argument("frames can not be empty");
        m_frames = frames;
    }

    void Spinner::setUpdateInterval(int ms)
    {
        m_minUpdateIntervalMs = ms;
    }

    void Spinner::setSpinnerPosition(SpinnerPosition pos)
    {
        m_spinnerPosition = pos;
    }

    std::string Spinner::draw(const StyleContext& ctx) const {
        if (m_finished) {
            return m_text;
        }

        std::string output;

        output = detail::color_to_ansi(m_color);

        if (!m_text.empty()) {
            switch (m_spinnerPosition)
            {
            case SpinnerPosition::Left:
                output += m_frames[m_currentFrame] + " " + m_text;
                break;
            case SpinnerPosition::Right:
                output += m_text + " " + m_frames[m_currentFrame];
                break;
            }
        }
        else {
            output += m_frames[m_currentFrame];
        }

        output += ctx.apply();

        return output;
    }

    void Spinner::update() {
        if (m_finished) return;

        auto now = detail::GET_NOW();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastUpdate).count();

        if (elapsed > m_minUpdateIntervalMs) {
            m_currentFrame = (m_currentFrame + 1) % m_frames.size();

            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << detail::CLEAR_LINE << draw() << std::flush;
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
            std::cout << detail::CLEAR_LINE << draw() << "\n" << std::flush;
        }
    }
}