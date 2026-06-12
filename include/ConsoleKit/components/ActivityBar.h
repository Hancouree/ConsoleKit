#pragma once
#include <string>
#include <chrono>
#include "../core/Component.h"
#include "../core/Common.h"

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
		void setShowBorders(bool show);
		void setPosition(Position p);
		void finish(const std::string& message = "Done");

		std::string draw(const StyleContext& ctx = {}) const override;
		void tick() override;
	private:
		std::string drawMarquee() const;
		std::string drawPulse() const;
		std::string drawBounce() const;

		int m_width;
		int m_currentFrame;
		int m_delta;
		Style m_style;
		Position m_position;
		bool m_showBorders;

		bool m_isFinished;
		std::string m_finishMessage;
		std::string m_text;

		static constexpr char ASCII_MARQUEE_RIGHT = '>';
		static constexpr char ASCII_MARQUEE_LEFT = '<';
		static constexpr const char* UNICODE_MARQUEE_RIGHT = "►";
		static constexpr const char* UNICODE_MARQUEE_LEFT = "◄";
		
		static constexpr char ASCII_PULSE = '=';
		static constexpr const char* UNICODE_PULSE = "█";

		static constexpr char ASCII_BOUNCE = 'O';
		static constexpr const char* UNICODE_BOUNCE = "●";
	};
}

