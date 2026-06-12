#include "../../include/ConsoleKit/components/VSeparator.h"
#include "../../include/ConsoleKit/core/Common.h"
#include <stdexcept>

ck::VSeparator::VSeparator(const std::string& label, Container* parent) 
	: StyledComponent(parent)
	, m_label(label)
	, m_height(DEFAULT_HEIGHT)
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

void ck::VSeparator::setTheme(Theme theme)
{
	m_theme = theme;
	m_isDirty = true;
}

std::string ck::VSeparator::draw(const StyleContext& ctx) const
{
	if (m_isDirty) {
		std::string color = detail::color_to_ansi(m_color);
		std::string output;

		std::vector<std::string> lines = getLines(m_label);
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

std::vector<std::string> ck::VSeparator::getLines(const std::string& raw) const
{
	std::vector<std::string> output;

	std::string fillChar = m_theme == Theme::Ascii 
		? ASCII_SYMBOL 
		: UNICODE_SYMBOL;

	if (raw.empty()) {
		output.resize(m_height, fillChar);
		return output;
	}

	output.reserve(m_height);
	int labelLen = detail::visible_length(raw);
	int total = m_height - labelLen - 2;

	if (total <= 0) {
		for (int i = 0; i < m_height; ++i) {
			output.push_back({ raw[i] });
		}
	}
	else {
		int above = total / 2, below = total - above;
		output.insert(output.end(), above, fillChar);
		output.push_back({ SPACE });
		for (int i = 0; i < labelLen; ++i) {
			output.push_back({ raw[i] });
		}
		output.push_back({ SPACE });
		output.insert(output.end(), below, fillChar);
	}

	return output;
}
