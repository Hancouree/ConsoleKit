#include "../../include/ConsoleKit/components/Spinner.h"

ck::Spinner::Spinner(const std::string& text, Container* parent)
    : StyledComponent(parent)
    , m_currentFrame(0)
    , m_finished(false)
    , m_position(Position::Right)
    , m_text(text)
    , m_frames({ SPINNER_PIPE, SPINNER_SLASH, SPINNER_DASH, SPINNER_BACKSLASH })
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
    std::string color = detail::color_to_ansi(m_color);

    if (m_finished) {
        return color + m_text + ctx.apply();
    }

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
    m_currentFrame = (m_currentFrame + 1) % m_frames.size();
}
