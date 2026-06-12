#include "../../include/ConsoleKit/components/ActivityBar.h"
#include <stdexcept>

ck::ActivityBar::ActivityBar(const std::string& text, Container* parent)
	: StyledComponent(parent)
	, m_width(50)
	, m_currentFrame(0)
	, m_delta(1)
	, m_style(Style::Marquee)
	, m_position(Position::Right)
	, m_showBorders(true)
	, m_isFinished(false)
	, m_text(text)
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

void ck::ActivityBar::setShowBorders(bool show)
{
	m_showBorders = show;
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

	if (m_showBorders) output += '[';
	
	switch (m_style) {
	case Style::Marquee: output += drawMarquee(); break;
	case Style::Pulse: output += drawPulse();   break;
	case Style::Bounce: output += drawBounce();  break;
	}

	if (m_showBorders) output += ']';

	if (m_position == Position::Right && !m_text.empty()) {
		output += " " + m_text;
	}

	return output + ctx.apply();
}

void ck::ActivityBar::tick()
{
	if (m_isFinished) return;

	m_currentFrame += m_delta;
	if (m_currentFrame >= m_width || m_currentFrame < 0) {
		m_delta *= -1;
		m_currentFrame += m_delta * 2;
	}
}

std::string ck::ActivityBar::drawMarquee() const
{
	std::string s;
	for (int i = 0; i < m_width; ++i) {
		if (i != m_currentFrame) {
			s += ' ';
			continue;
		}

		if (m_theme == Theme::Ascii) {
			s += m_delta > 0 ? ASCII_MARQUEE_RIGHT : ASCII_MARQUEE_LEFT;
		}
		else {
			s += m_delta > 0 ? UNICODE_MARQUEE_RIGHT : UNICODE_MARQUEE_LEFT;
		}
	}

	return s;
}

std::string ck::ActivityBar::drawPulse() const
{
	std::string s;
	s.reserve(m_currentFrame);

	std::string current = m_theme == Theme::Ascii 
		? std::string{ ASCII_PULSE } 
		: UNICODE_PULSE;
	
	for (int i = 0; i < m_currentFrame; ++i) {
		s += current;
	}

	if (m_delta > 0) {
		return s + std::string(m_width - m_currentFrame, ' ');
	}
	else {
		return std::string(m_width - m_currentFrame, ' ') + s;
	}
}

std::string ck::ActivityBar::drawBounce() const
{
	std::string s(m_currentFrame, ' ');

	s += m_theme == Theme::Ascii
		? std::string{ ASCII_BOUNCE }
		: UNICODE_BOUNCE;

	s += std::string(m_width - m_currentFrame - 1, ' ');
	return s;
}
