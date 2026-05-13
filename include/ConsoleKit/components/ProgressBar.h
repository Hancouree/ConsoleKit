#pragma once
#include <string>
#include <chrono>
#include "../core/Component.h"

namespace ck {
	class ProgressBar final : public StyledComponent
	{
	public:
		ProgressBar(int current, int total, Container* parent = nullptr);

		void setWidth(int width);
		void setText(const std::string& text);
		void update(int current);
		void increment(int delta = 1);

		ProgressBar& withPercent(bool enable = true);
		ProgressBar& withSpeed(bool enable = true);
		ProgressBar& withETA(bool enable = true);

		std::string draw(const StyleContext& ctx = {}) const override;
	private:
		int getPercent() const;
		double getSpeed() const;
		int getTimeLeft() const;
		std::string formatTime(int seconds) const;

		int m_width = 50;
		int m_current;
		int m_total;
		std::string m_text;

		std::chrono::steady_clock::time_point m_startTime;

		bool m_showPercent = false;
		bool m_showSpeed = false;
		bool m_showETA = false;
	};
}

