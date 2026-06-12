#include "../../include/ConsoleKit/components/ProgressBar.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

ck::ProgressBar::ProgressBar(int current, int total, Container* parent)
	: StyledComponent(parent)
	, m_width(DEFAULT_PROGRESSBAR_WIDTH)
	, m_current(current)
	, m_total(total)
	, m_startTime(detail::GET_NOW())
	, m_isCompleted(false)
{
	if (total <= 0) throw std::invalid_argument("total must be greater than zero");
	if (m_current < 0 || m_current > total) throw std::invalid_argument("current out of range");

	m_columns.push_back({ ColumnType::Text, "" });
	m_columns.push_back({ ColumnType::Bar, "" });
	m_columns.push_back({ ColumnType::Percent, "" });
}

void ck::ProgressBar::setWidth(int width)
{
	if (width <= 0) throw std::invalid_argument("Invalid width value");
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

void ck::ProgressBar::setColumns(const std::vector<Column>& columns)
{
	m_columns = columns;
}

void ck::ProgressBar::onComplete(std::function<void()> cb)
{
	m_onComplete = cb;
}

std::string ck::ProgressBar::draw(const StyleContext& ctx) const
{
	std::string output;
	output += detail::color_to_ansi(m_color);

	for (const auto& c : m_columns) {
		switch (c.type)
		{
		case ColumnType::Text:
			if (!c.label.empty()) output += " " + c.label;
			if (!m_text.empty()) output += " " + m_text;
			break;
		case ColumnType::Bar:
			if (!c.label.empty()) output += " " + c.label + " ";
			if (m_theme == Theme::Ascii) {
				output += drawBarAscii();
			}
			else {
				output += drawBarUnicode();
			}
			break;
		case ColumnType::Percent:
			if (!c.label.empty()) output += " " + c.label;
			output += " " + std::to_string(getPercent()) + '%';
			break;
		case ColumnType::Eta:
			if (!c.label.empty()) output += " " + c.label;
			output += " " + formatTime(getTimeLeft());
			break;
		case ColumnType::Elapsed:
			if (!c.label.empty()) output += " " + c.label;
			output += " " + formatTime(getElapsedSeconds());
			break;
		case ColumnType::Speed: {
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(1) << getSpeed();
			if (!c.label.empty()) output += " " + c.label;
			output += " " + oss.str() + " it/s";
			break;
		}
		}
	}

	return output + ctx.apply();
}

int ck::ProgressBar::getPercent() const
{
	return static_cast<int>(m_current * 100.0 / m_total);
}

double ck::ProgressBar::getSpeed() const
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

std::string ck::ProgressBar::drawBarAscii() const
{
	double progress = static_cast<double>(m_current) / m_total;
	int fullBlocks = static_cast<int>(progress * m_width);

	std::string output;

	output += ASCII_START;
	for (int i = 0; i < m_width - 2; ++i) {
		if (i < fullBlocks) {
			output += ASCII_FILL;
		}
		else if (i == fullBlocks) {
			output += ASCII_HEAD;
		}
		else {
			output += ' ';
		}
	}
	output += ASCII_END;

	return output;
}

std::string ck::ProgressBar::drawBarUnicode() const
{
	double progress = static_cast<double>(m_current) / m_total;
	int fullBlocks = static_cast<int>(progress * m_width);

	std::string output;
	for (int i = 0; i < fullBlocks && i < m_width; ++i) {
		output += UNICODE_FILL;
	}

	if (m_current < m_total) {
		int emptyCount = m_width - fullBlocks - 1;
		for (int i = 0; i < emptyCount; ++i) {
			output += " ";
		}
	}

	return output;
}
