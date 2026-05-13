#include "../../include/ConsoleKit/components/Table.h"
#include <stdexcept>

ck::Table::Table(const std::vector<std::string>& columns, Container* parent)
	: StyledComponent(parent)
	, m_columns(columns)
	, m_isDirty(true)
{
	m_fixedColumnWidths.resize(m_columns.size(), 0);
}

void ck::Table::addRow(const std::vector<std::string>& row)
{
	if (row.size() != m_columns.size()) {
		throw std::invalid_argument("Row size doesn't match column count");
	}
	
	m_rows.push_back(row);
	m_isDirty = true;
}

void ck::Table::removeRow(int index)
{
	if (index < 0 || index >= m_rows.size()) {
		throw std::invalid_argument("Invalid index");
	}

	m_rows.erase(m_rows.begin() + index);
	m_isDirty = true;
}

void ck::Table::setColumnWidth(int index, int width)
{
	if (index < 0 || index >= m_columns.size()) {
		throw std::invalid_argument("Invalid index");
	}

	if (width <= 0) throw std::invalid_argument("Invalid width value");

	m_fixedColumnWidths[index] = width;
	m_isDirty = true;
}

void ck::Table::setCell(int row, int column, const std::string& text)
{
	if (row < 0 || row >= m_rows.size()) {
		throw std::invalid_argument("Invalid row value");
	}

	if (column < 0 || column >= m_columns.size()) {
		throw std::invalid_argument("Invalid column value");
	}

	m_rows[row][column] = text;
	m_isDirty = true;
}

void ck::Table::setColor(Color c)
{
	m_color = c;
	m_isDirty = true;
}

void ck::Table::clear()
{
	m_rows.clear();
	m_isDirty = true;
}

std::string ck::Table::draw(const StyleContext& ctx) const
{
	if (!m_isDirty) {
		return m_cachedOutput + ctx.apply();
	}

	auto widths = getColumnsWidth();
	std::string tColor = detail::color_to_ansi(m_color);
	std::string output;
	std::string line = drawLine(widths, '-');

	output += tColor + (line.empty() ? "" : line + "\n");
	output += tColor + drawRow(m_columns, widths) + "\n";
	output += tColor + (line.empty() ? "" : line + "\n");
	for (const auto& row : m_rows) {
		output += tColor + drawRow(row, widths) + "\n";
	}
	if (!m_rows.empty()) {
		output += tColor + line;
	}

	output += detail::RESET; 
	m_cachedOutput = output; 
	m_isDirty = false;

	return output + ctx.apply(); 
}

int ck::Table::getHeight() const
{
	int rowsSize = m_rows.size();
	return rowsSize > 0 ? 4 + rowsSize : 3;
}

std::vector<int> ck::Table::getColumnsWidth() const
{
	std::vector<int> widths;
	for (size_t col = 0; col < m_columns.size(); ++col) {
		int fixedValue = m_fixedColumnWidths[col];
		if (fixedValue != 0) {
			widths.push_back(fixedValue);
			continue;
		}

		int w = detail::visible_length(m_columns[col]);
		for (const auto& row : m_rows) {
			w = std::max(w, (int)detail::visible_length(row[col]));
		}

		widths.push_back(w);
	}

	return widths;
}

std::string ck::Table::pad(const std::string& str, int width) const
{
	int len = detail::visible_length(str);
	int total = width - len;
	int left = total / 2;
	int right = total - left;
	return std::string(left, ' ') + str + std::string(right, ' ');
}

std::string ck::Table::drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const
{
	std::string output = "|";
	for (size_t i = 0; i < cells.size(); ++i) {
		output += ' ';
		output += pad(cells[i], widths[i]);
		output += ' ';
		output += (i + 1 < cells.size()) ? "|" : "|";
	}
	return output;
}

std::string ck::Table::drawLine(const std::vector<int>& widths, char sep) const
{
	std::string output = "+";
	for (size_t i = 0; i < widths.size(); ++i) {
		output += std::string(widths[i] + 2, '-');
		output += (i + 1 < widths.size()) ? "+" : "+";
	}
	return output;
}
