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
		enum class ColumnType
		{
			Text,
			Bar,
			Percent,
			Speed,
			Eta,
			Elapsed
		};

		struct Column {
			ColumnType type;
			std::string label;
		};

		ProgressBar(int current, int total, Container* parent = nullptr);

		void setWidth(int width);
		void setText(const std::string& text);
		void update(int current);
		void increment(int delta = 1);
		void setColumns(const std::vector<Column>& columns);

		void onComplete(std::function<void()> cb);

		std::string draw(const StyleContext& ctx = {}) const override;
	private:
		static constexpr int DEFAULT_PROGRESSBAR_WIDTH = 50;

		struct Sample
		{
			std::chrono::steady_clock::time_point tp;
			int value;
		};

		int getPercent() const;
		double getSpeed() const;
		int getTimeLeft() const;
		int getElapsedSeconds() const;
		std::string formatTime(int seconds) const;
		std::string drawBarAscii() const;
		std::string drawBarUnicode() const;

		int m_width;
		int m_current;
		int m_total;
		std::string m_text;
		std::vector<Column> m_columns;

		std::chrono::steady_clock::time_point m_startTime;
		std::chrono::steady_clock::time_point m_endTime;

		mutable std::list<Sample> m_samples;
		static constexpr int WINDOW_SIZE = 10;

		std::function<void()> m_onComplete;
		bool m_isCompleted;

		static constexpr char ASCII_START = '[';
		static constexpr char ASCII_END = ']';
		static constexpr char ASCII_FILL = '=';
		static constexpr char ASCII_HEAD = '>';

		static constexpr const char* UNICODE_FILL = "█";
	};
}

