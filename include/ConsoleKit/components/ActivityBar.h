#pragma once
#include <string>
#include <chrono>
#include "../core/Component.h"

namespace ck {
	class ActivityBar final : public StyledComponent
	{
	public:
		enum class Style { Marquee, Pulse, Bounce };
		enum class Position { Left, Right };

		ActivityBar(const std::string& text = "", Container * parent = nullptr);

		void setWidth(int width);
		void setStyle(Style s);
		void setText(const std::string& text);
		void setUpdateInterval(int ms);
		void setPosition(Position p);
		void finish(const std::string& message = "Done");

		std::string draw(const StyleContext& ctx = {}) const override;
		void tick() override;
	private:
		std::string drawMarquee() const;
		std::string drawPulse() const;
		std::string drawBounce() const;

		int m_intervalMs = 50;
		int m_width = 50;
		int m_currentFrame = 0;
		int m_delta = 1;
		Style m_style = Style::Marquee;
		Position m_position = Position::Right;

		bool m_isFinished;
		std::string m_finishMessage;

		std::string m_text;
		std::chrono::steady_clock::time_point m_lastTick;
	};
}

