#include "../../include/ConsoleKit/components/ActivityBar.h"
#include <stdexcept>

ck::ActivityBar::ActivityBar(const std::string& text, Container* parent)
	: StyledComponent(parent)
	, m_isFinished(false)
	, m_text(text)
	, m_lastTick(detail::GET_NOW())
{
}

void ck::ActivityBar::setWidth(int width)
{
	if (width <= 0) throw std::invalid_argument("Invalid width value");
	m_width = width;
}

void ck::ActivityBar::setStyle(Style s)
{
	m_style = s;
}

void ck::ActivityBar::setText(const std::string& text)
{
	m_text = text;
}

void ck::ActivityBar::setUpdateInterval(int ms)
{
	if (ms <= 0) throw std::invalid_argument("interval must be positive");
	m_intervalMs = ms;
}

void ck::ActivityBar::setPosition(Position p)
{
	m_position = p;
}

void ck::ActivityBar::finish(const std::string& message)
{
	m_finishMessage = message;
	m_isFinished = true;
}

std::string ck::ActivityBar::draw(const StyleContext& ctx) const
{
	if (m_isFinished) {
		return m_finishMessage + ctx.apply();
	}

	std::string output;
	output += detail::color_to_ansi(m_color);

	if (m_position == Position::Left && !m_text.empty()) {
		output += m_text + " ";
	}

	output += '[';
	switch (m_style) {
	case Style::Marquee: output += drawMarquee(); break;
	case Style::Pulse: output += drawPulse();   break;
	case Style::Bounce: output += drawBounce();  break;
	}
	output += ']';

	if (m_position == Position::Right && !m_text.empty()) {
		output += " " + m_text;
	}

	return output + ctx.apply();
}

void ck::ActivityBar::tick()
{
	if (m_isFinished) return;

	auto now = detail::GET_NOW();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastTick).count();

	if (elapsed >= m_intervalMs) {
		m_currentFrame += m_delta;
		if (m_currentFrame >= m_width || m_currentFrame < 0) {
			m_delta *= -1;
			m_currentFrame += m_delta * 2;
		}

		m_lastTick = now;
	}
}

std::string ck::ActivityBar::drawMarquee() const
{
	std::string s(m_width, ' ');
	s[m_currentFrame] = m_delta > 0 ? '>' : '<';
	return s;
}

std::string ck::ActivityBar::drawPulse() const
{
	if (m_delta > 0) {
		return std::string(m_currentFrame, '=') + std::string(m_width - m_currentFrame, ' ');
	}
	else {
		return std::string(m_width - m_currentFrame, ' ') + std::string(m_currentFrame, '=');
	}
}

std::string ck::ActivityBar::drawBounce() const
{
	std::string s(m_width, ' ');
	s[m_currentFrame] = 'O';
	return s;
}
