#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../core/Component.h"

namespace ck {
	class Table final : public StyledComponent
	{
	public:
		enum class ColumnAlign { Left, Center, Right };

		Table(const std::vector<std::string>& columns, Container* parent = nullptr);

		void setHeaderVisible(bool visible);
		void setColumnAlign(int index, ColumnAlign align);
		void addRow(const std::vector<std::string>& row);
		void setRowColor(int index, Color c);
		void setAlternatingColors(Color odd, Color even);
		void removeRow(int index);
		void setColumnWidth(int index, int width);
		void setCell(int row, int column, const std::string& text);
		void setColor(Color c) override;
		void setTheme(Theme theme) override;
		void clear();
		void clearAlternatingColors();

		std::string draw(const StyleContext& ctx = {}) const override;
		int getHeight() const override;
		std::vector<int> getColumnsWidth() const;
	private:
		struct Column {
			std::string value;
			ColumnAlign align;
			int fixedWidth;
		};

		struct Row {
			std::vector<std::string> values;
			std::optional<Color> color;
		};

		struct AlternatingColors {
			std::optional<Color> odd;
			std::optional<Color> even;
		};

		std::string pad(const std::string& str, int width, ColumnAlign align) const;
		std::string drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const;
		std::string drawRowUnicode(const std::vector<std::string>& cells, const std::vector<int>& widths) const;
		std::string drawLine(const std::vector<int>& widths) const;
		std::string drawLineUnicode(const std::vector<int>& widths,	const std::string& left_joint, const std::string& middle_joint, const std::string& right_joint) const;
		std::string drawUpperLineUnicode(const std::vector<int>& widths) const;
		std::string drawIntermediateLineUnicode(const std::vector<int>& widths) const;
		std::string drawLowerLineUnicode(const std::vector<int>& widths) const;

		bool m_visibleHeader;
		std::vector<Column> m_columns;
		std::vector<Row> m_rows;
		AlternatingColors m_altColors;

		static constexpr char ASCII_HORIZONTAL = '-';
		static constexpr char ASCII_VERTICAL = '|';
		static constexpr char ASCII_CROSS = '+';

		static constexpr const char* UNICODE_TOP_LEFT = "╔";
		static constexpr const char* UNICODE_TOP_T = "╦";
		static constexpr const char* UNICODE_TOP_RIGHT = "╗";
		static constexpr const char* UNICODE_LEFT_T = "╠";
		static constexpr const char* UNICODE_CROSS = "╬";
		static constexpr const char* UNICODE_RIGHT_T = "╣";
		static constexpr const char* UNICODE_BOTTOM_LEFT = "╚";
		static constexpr const char* UNICODE_BOTTOM_T = "╩";
		static constexpr const char* UNICODE_BOTTOM_RIGHT = "╝";
		static constexpr const char* UNICODE_HORIZONTAL = "═";
		static constexpr const char* UNICODE_VERTICAL = "║";
	};
}

