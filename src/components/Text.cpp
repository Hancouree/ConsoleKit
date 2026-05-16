#include "../../include/ConsoleKit/components/Text.h"

ck::Text::Text(const std::string& value, Container* parent)
	: StyledComponent(parent)
	, m_align(Align::Left)
	, m_maxWidth(DEFAULT_MAX_WIDTH)
    , m_lastHeight(0)
	, m_isDirty(true)
{
    setText(value);
}

void ck::Text::setText(const std::string& value)
{
    m_rawText = value;
    m_isDirty = true;
}

void ck::Text::setAlign(Align align)
{
	m_align = align;
    m_isDirty = true;
}

void ck::Text::setOverflow(Overflow overflow)
{
    m_overflow = overflow;
    m_isDirty = true;
}

void ck::Text::setMaxWidth(int width)
{
	m_maxWidth = width;
    m_isDirty = true;
}

std::string ck::Text::draw(const StyleContext& ctx) const
{
    if (m_isDirty) {
        std::vector<std::string> lines = getLines(m_rawText);

        std::string tColor = detail::color_to_ansi(m_color);
        std::string output;

        size_t n = lines.size();
        for (size_t i = 0; i < n; ++i) {
            output += tColor;

            if (m_align == Align::Left) {
                output += lines[i];
            }
            else if (m_align == Align::Right) {
                output += std::string(m_maxWidth - detail::visible_length(lines[i]), ' ') + lines[i];
            }
            else {
                int total = m_maxWidth - detail::visible_length(lines[i]);
                if (total <= 0) output += lines[i];
                else output += std::string(total / 2, ' ') + lines[i] + std::string(total - (total / 2), ' ');
            }

            output += detail::RESET;
            if (i != n - 1) output += "\n";
        }

        m_lastHeight = n;
        m_cachedOutput = output;
        m_isDirty = false;
    }

    return m_cachedOutput + ctx.apply();
}

std::vector<std::string> ck::Text::getLines(std::string raw) const
{
    std::vector<std::string> output;
    auto paragraphs = detail::splitLines(raw);

    for (const auto& paragraph : paragraphs) {
        if (paragraph.empty()) {
            output.push_back("");
            continue;
        }
        
        std::string remaining = paragraph;
        while (!remaining.empty()) {
            if (m_maxWidth <= 0 || detail::visible_length(remaining) <= m_maxWidth) {
                output.push_back(std::move(remaining));
                break;  
            }

            if (m_overflow == Overflow::Truncate) {
                output.push_back(remaining.substr(0, m_maxWidth - 3) + "...");
                return output;
            }

            size_t lastSpaceIdx = std::string::npos;
            for (size_t i = 0; i < (size_t)m_maxWidth && i < remaining.size(); ++i) {
                if (remaining[i] == ' ') lastSpaceIdx = i;
            }

            if (lastSpaceIdx != std::string::npos) {
                output.push_back(remaining.substr(0, lastSpaceIdx));
                remaining = remaining.substr(lastSpaceIdx + 1);
            }
            else {
                output.push_back(remaining.substr(0, m_maxWidth));
                remaining = remaining.substr(m_maxWidth);
            }
        }
    }

    return output;
}
