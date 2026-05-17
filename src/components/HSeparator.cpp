#include "../../include/ConsoleKit/components/HSeparator.h"
#include "../../include/ConsoleKit/core/Common.h"
#include <stdexcept>

ck::HSeparator::HSeparator(const std::string& label, Container* parent)
	: StyledComponent(parent)
	, m_label(label)
	, m_width(DEFAULT_WIDTH)
	, m_char('-')
	, m_isDirty(true)
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

void ck::HSeparator::setChar(char c)
{
	m_char = c;
	m_isDirty = true;
}

std::string ck::HSeparator::draw(const StyleContext& ctx) const
{
	if (m_isDirty) {
		std::string color = detail::color_to_ansi(m_color);

		std::string output = color;
		if (m_label.empty()) {
			output += std::string(m_width, m_char);
		}
		else {
			int labelLen = detail::visible_length(m_label);
			int fill = m_width - labelLen - 2;
			if (fill <= 0) {
				output += m_label;
			}
			else {
				int left = fill / 2, right = fill - (fill / 2);
				output += std::string(left, m_char) + ' ' + m_label + ' ' + std::string(right, m_char);
			}
		}

		m_cachedOutput = output;
		m_isDirty = false;
	}

	return m_cachedOutput + ctx.apply();
}
