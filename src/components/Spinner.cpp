#include "../../include/ConsoleKit/components/Spinner.h"

ck::Spinner::Spinner(const std::string& text, Container* parent)
    : StyledComponent(parent)
    , m_text(text)
    , m_frames({ "|", "/", "-", "\\" })
    , m_lastTick(detail::GET_NOW())
{
}

void ck::Spinner::setText(const std::string& text)
{
    m_text = text;
}

void ck::Spinner::setFrames(const std::vector<std::string>& frames)
{
    if (frames.empty()) throw std::invalid_argument("frames cannot be empty");
    m_frames = frames;
}

void ck::Spinner::setUpdateInterval(int ms)
{
    if (ms <= 0) throw std::invalid_argument("interval must be positive");
    m_intervalMs = ms;
}

void ck::Spinner::setPosition(Position pos)
{
    m_position = pos;
}

void ck::Spinner::finish(const std::string& message)
{
    m_finished = true;
    m_text = message;
}

std::string ck::Spinner::draw(const StyleContext& ctx) const
{
    if (m_finished) {
        return m_text + ctx.apply();
    }

    std::string color = detail::color_to_ansi(m_color);
    const std::string& frame = m_frames[m_currentFrame];

    std::string output = color;
    if (m_position == Position::Left) {
        output += frame + (m_text.empty() ? "" : " " + m_text);
    }
    else {
        output += (m_text.empty() ? "" : m_text + " ") + frame;
    }

    return output + ctx.apply();
}

void ck::Spinner::tick()
{
    if (m_finished) return;

    auto now = detail::GET_NOW();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastTick).count();

    if (elapsed >= m_intervalMs) {
        m_currentFrame = (m_currentFrame + 1) % m_frames.size();
        m_lastTick = now;
    }
}
