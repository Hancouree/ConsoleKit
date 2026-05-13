#include "../../include/ConsoleKit/components/ProgressBar.h"
#include <stdexcept>

ck::ProgressBar::ProgressBar(int current, int total, Container* parent)
	: StyledComponent(parent)
	, m_current(current)
	, m_total(total)
	, m_startTime(detail::GET_NOW())
{
	if (total <= 0) throw std::invalid_argument("total must be greater than zero");
	if (m_current < 0 || m_current > total) throw std::invalid_argument("current out of range");
}

void ck::ProgressBar::setWidth(int width)
{
	if (width < 0) throw std::invalid_argument("Invalid width value");
	m_width = width;
}

void ck::ProgressBar::setText(const std::string& text)
{
	m_text = text;
}

void ck::ProgressBar::update(int current)
{
	m_current = std::min(current, m_total);
}

void ck::ProgressBar::increment(int delta)
{
	update(m_current + delta);
}

ck::ProgressBar& ck::ProgressBar::withPercent(bool enable)
{
	m_showPercent = enable;
	return *this;
}

ck::ProgressBar& ck::ProgressBar::withSpeed(bool enable)
{
	m_showSpeed = enable;
	return *this;
}

ck::ProgressBar& ck::ProgressBar::withETA(bool enable)
{
	m_showETA = enable;
	return *this;
}

std::string ck::ProgressBar::draw(const StyleContext& ctx) const
{
	int filled = static_cast<double>(m_current) / m_total * m_width;

	std::string output;
	output += detail::color_to_ansi(m_color);
	if (!m_text.empty()) output += m_text + " ";

	output += '[';
	for (int i = 0; i < m_width; ++i) {
		if (i < filled) output += '=';
		else if (i == filled) output += '>';
		else output += ' ';
	}
	output += ']';

	if (m_showPercent) output += " " + std::to_string(getPercent()) + '%';
	if (m_showSpeed) output += " " + std::to_string(static_cast<int>(getSpeed())) + " its/s";
	if (m_showETA) output += " " + formatTime(getTimeLeft());

	return output + ctx.apply();
}

int ck::ProgressBar::getPercent() const
{
	return static_cast<int>(m_current * 100.0 / m_total);
}

double ck::ProgressBar::getSpeed() const
{
	auto elapsed = std::chrono::duration<double>(detail::GET_NOW() - m_startTime).count();
	if (elapsed < 0.001) return 0;
	return m_current / elapsed;
}

int ck::ProgressBar::getTimeLeft() const
{
	double speed = getSpeed();
	if (speed < 0.001) return 9999;
	return static_cast<int>((m_total - m_current) / speed);
}

std::string ck::ProgressBar::formatTime(int seconds) const
{
	if (seconds < 0) return "??:??";
	if (seconds < 60) return std::to_string(seconds) + "s";
	if (seconds < 3600) return std::to_string(seconds / 60) + "m " + std::to_string(seconds % 60) + "s";
	return std::to_string(seconds / 3600) + "h " + std::to_string((seconds % 3600) / 60) + "m";
}
