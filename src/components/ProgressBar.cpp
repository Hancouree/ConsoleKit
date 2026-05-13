#include "../../include/ConsoleKit/components/ProgressBar.h"
#include <stdexcept>

ck::ProgressBar::ProgressBar(int current, int total, Container* parent)
	: StyledComponent(parent)
	, m_current(current)
	, m_total(total)
	, m_startTime(detail::GET_NOW())
	, m_fillChar('=')
	, m_headChar('>')
	, m_emptyChar(' ')
	, m_isCompleted(false)
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

	auto tp = detail::GET_NOW();

	m_samples.push_back({ tp, m_current });
	if (m_samples.size() > WINDOW_SIZE) {
		m_samples.pop_front();
	}

	if (m_current == m_total && !m_isCompleted) {
		m_isCompleted = true;
		m_endTime = tp;
		if (m_onComplete) m_onComplete();
	}
}

void ck::ProgressBar::increment(int delta)
{
	update(m_current + delta);
}

void ck::ProgressBar::setFillChar(char c)
{
	m_fillChar = c;
}

void ck::ProgressBar::setHeadChar(char c)
{
	m_headChar = c;
}

void ck::ProgressBar::setEmptyChar(char c)
{
	m_emptyChar = c;
}

void ck::ProgressBar::onComplete(std::function<void()> cb)
{
	m_onComplete = cb;
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

ck::ProgressBar& ck::ProgressBar::withElapsed(bool enable)
{
	m_showElapsed = enable;
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
		if (i < filled) output += m_fillChar;
		else if (i == filled) output += m_headChar;
		else output += m_emptyChar;
	}
	output += ']';

	if (m_showPercent) output += " " + std::to_string(getPercent()) + '%';
	if (m_showSpeed) output += " " + std::to_string(getSpeed()) + " its/s";
	if (m_showETA) output += " " + formatTime(getTimeLeft());
	if (m_showElapsed) output += " " + formatTime(getElapsedSeconds());

	return output + ctx.apply();
}

int ck::ProgressBar::getPercent() const
{
	return static_cast<int>(m_current * 100.0 / m_total);
}

int ck::ProgressBar::getSpeed() const
{
	if (m_samples.size() < 2) {
		auto elapsed = std::chrono::duration<double>(detail::GET_NOW() - m_startTime).count();
		if (elapsed < 0.001) return 0;
		return m_current / elapsed;
	}

	auto& oldest = m_samples.front();
	auto& newest = m_samples.back();

	auto elapsed = std::chrono::duration<double>(newest.tp - oldest.tp).count();
	if (elapsed < 0.001) return 0;

	return (newest.value - oldest.value) / elapsed;
}

int ck::ProgressBar::getTimeLeft() const
{
	if (m_isCompleted) {
		return 0;
	}

	double speed = getSpeed();
	if (speed < 0.001) return 9999;
	return static_cast<int>((m_total - m_current) / speed);
}

int ck::ProgressBar::getElapsedSeconds() const
{
	if (m_isCompleted) {
		return std::chrono::duration_cast<std::chrono::seconds>(m_endTime - m_startTime).count();
	}

	return std::chrono::duration_cast<std::chrono::seconds>(detail::GET_NOW() - m_startTime).count();
}

std::string ck::ProgressBar::formatTime(int seconds) const
{
	if (seconds < 0) return "??:??";
	if (seconds < 60) return std::to_string(seconds) + "s";
	if (seconds < 3600) return std::to_string(seconds / 60) + "m " + std::to_string(seconds % 60) + "s";
	return std::to_string(seconds / 3600) + "h " + std::to_string((seconds % 3600) / 60) + "m";
}
