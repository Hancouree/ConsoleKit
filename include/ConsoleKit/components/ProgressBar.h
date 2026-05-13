#pragma once
#include <string>
#include <chrono>
#include <functional>
#include <list>
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

		void setFillChar(char c);
		void setHeadChar(char c);
		void setEmptyChar(char c);

		void onComplete(std::function<void()> cb);

		ProgressBar& withPercent(bool enable = true);
		ProgressBar& withSpeed(bool enable = true);
		ProgressBar& withETA(bool enable = true);
		ProgressBar& withElapsed(bool enable = true);

		std::string draw(const StyleContext& ctx = {}) const override;
	private:
		struct Sample
		{
			std::chrono::steady_clock::time_point tp;
			int value;
		};

		int getPercent() const;
		int getSpeed() const;
		int getTimeLeft() const;
		int getElapsedSeconds() const;
		std::string formatTime(int seconds) const;

		int m_width = 50;
		int m_current;
		int m_total;
		std::string m_text;

		std::chrono::steady_clock::time_point m_startTime;
		std::chrono::steady_clock::time_point m_endTime;

		mutable std::list<Sample> m_samples;
		static constexpr int WINDOW_SIZE = 10;

		char m_fillChar;
		char m_headChar;
		char m_emptyChar;

		std::function<void()> m_onComplete;
		bool m_isCompleted;

		bool m_showPercent = false;
		bool m_showSpeed = false;
		bool m_showETA = false;
		bool m_showElapsed = false;
	};
}

