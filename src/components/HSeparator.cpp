#include "../../include/ConsoleKit/components/HSeparator.h"
#include <stdexcept>

ck::HSeparator::HSeparator(const std::string& label, Container* parent)
	: StyledComponent(parent)
	, m_label(label)
	, m_width(DEFAULT_WIDTH)
{
}

void ck::HSeparator::setLabel(const std::string& label)
{
	m_label = label;
	m_isDirty = true;
}

void ck::HSeparator::setWidth(int width)
{
	if (width <= 0) throw std::invalid_argument("Invalid width value");
	m_width = width;
	m_isDirty = true;
}

void ck::HSeparator::setTheme(Theme theme)
{
    m_theme = theme;
    m_isDirty = true;
}

std::string ck::HSeparator::draw(const StyleContext& ctx) const
{
    if (m_isDirty) {
        std::string color = detail::color_to_ansi(m_color);
        std::string fillChar = m_theme == Theme::Ascii ? ASCII_SYMBOL : UNICODE_SYMBOL;
        std::string output = color;

        if (m_label.empty()) {
            for (int i = 0; i < m_width; ++i) output += fillChar;
        }
        else {
            int labelLen = detail::visible_length(m_label);
            int fillLen = m_width - labelLen - 2;

            if (fillLen <= 0) {
                output += m_label;
            }
            else {
                int left = fillLen / 2;
                int right = fillLen - left;

                for (int i = 0; i < left; ++i) output += fillChar;
                output += " " + m_label + " ";
                for (int i = 0; i < right; ++i) output += fillChar;
            }
        }

        m_cachedOutput = output + ctx.apply();
        m_isDirty = false;
    }
    return m_cachedOutput;
}
