#include "../../include/ConsoleKit/components/VSeparator.h"
#include "../../include/ConsoleKit/core/Common.h"
#include <stdexcept>

ck::VSeparator::VSeparator(const std::string& label, Container* parent) 
	: StyledComponent(parent)
	, m_label(label)
	, m_height(DEFAULT_HEIGHT)
	, m_char('|')
	, m_isDirty(true)
{
}

void ck::VSeparator::setLabel(const std::string& label)
{
	m_label = label;
	m_isDirty = true;
}

void ck::VSeparator::setHeight(int height)
{
	if (height <= 0) throw std::invalid_argument("Invalid height value");
	m_height = height;
	m_isDirty = true;
}

void ck::VSeparator::setChar(char c)
{
	m_char = c;
	m_isDirty = true;
}

std::string ck::VSeparator::draw(const StyleContext& ctx) const
{
	if (m_isDirty) {
		std::string color = detail::color_to_ansi(m_color);
		std::string output;

		std::vector<char> lines = getLines(m_label);
		size_t n = lines.size();
		for (size_t i = 0; i < n; ++i) {
			output += color + lines[i] + detail::RESET;
			if (i != n - 1) output += "\n";
		}

		m_cachedOutput = output;
		m_isDirty = false;
	}

	return m_cachedOutput + ctx.apply();
}

std::vector<char> ck::VSeparator::getLines(const std::string& raw) const
{
	std::vector<char> output;

	if (raw.empty()) {
		output.resize(m_height, m_char);
		return output;
	}

	output.reserve(m_height);
	int labelLen = detail::visible_length(raw);
	int total = m_height - labelLen;

	if (total <= 0) {
		for (int i = 0; i < m_height; ++i) {
			output.push_back(raw[i]);
		}
	}
	else {
		int above = total / 2, below = total - above;
		output.insert(output.end(), above, m_char);
		for (int i = 0; i < labelLen; ++i) {
			output.push_back(raw[i]);
		}
		output.insert(output.end(), below, m_char);
	}

	return output;
}
