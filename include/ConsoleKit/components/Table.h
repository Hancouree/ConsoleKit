#pragma once
#include <string>
#include <vector>
#include "../core/Component.h"

namespace ck {
	class Table final : public StyledComponent
	{
	public:
		Table(const std::vector<std::string>& columns, Container* parent = nullptr);

		void addRow(const std::vector<std::string>& row);
		void removeRow(int index);
		void setColumnWidth(int index, int width);
		void setCell(int row, int column, const std::string& text);
		void setColor(Color c) override;
		void clear();

		std::string draw(const StyleContext& ctx = {}) const override;
		int getHeight() const override;
		std::vector<int> getColumnsWidth() const;
	private:
		std::string pad(const std::string& str, int width) const;
		std::string drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const;
		std::string drawLine(const std::vector<int>& widths, char sep) const;

		std::vector<std::string> m_columns;
		std::vector<std::vector<std::string>> m_rows;
		std::vector<int> m_fixedColumnWidths;

		mutable std::string m_cachedOutput;
		mutable bool m_isDirty;
	};
}

