#include "../../include/ConsoleKit/components/Table.h"
#include <stdexcept>
#include <ranges>

ck::Table::Table(const std::vector<std::string>& columns, Container* parent)
	: StyledComponent(parent)
	, m_visibleHeader(true)
{
	for (const auto& c : columns) {
		m_columns.push_back({ c, ColumnAlign::Center, 0 });
	}
}

void ck::Table::setHeaderVisible(bool visible)
{
	m_visibleHeader = visible;
	m_isDirty = true;
}

void ck::Table::setColumnAlign(int index, ColumnAlign align)
{
	if (index < 0 || index >= m_columns.size()) {
		throw std::invalid_argument("Invalid column index");
	}

	m_columns[index].align = align;
	m_isDirty = true;
}

void ck::Table::addRow(const std::vector<std::string>& row)
{
	if (row.size() != m_columns.size()) {
		throw std::invalid_argument("Row size doesn't match column count");
	}
	
	m_rows.push_back({ row, std::nullopt });
	m_isDirty = true;
}

void ck::Table::setRowColor(int index, Color c)
{
	if (index < 0 || index >= m_rows.size()) {
		throw std::invalid_argument("Invalid row index");
	}

	m_rows[index].color = c;
	m_isDirty = true;
}

void ck::Table::setAlternatingColors(Color odd, Color even)
{
	m_altColors.odd = odd;
	m_altColors.even = even;
	m_isDirty = true;
}

void ck::Table::removeRow(int index)
{
	if (index < 0 || index >= m_rows.size()) {
		throw std::invalid_argument("Invalid row index");
	}

	m_rows.erase(m_rows.begin() + index);
	m_isDirty = true;
}

void ck::Table::setColumnWidth(int index, int width)
{
	if (index < 0 || index >= m_columns.size()) {
		throw std::invalid_argument("Invalid column index");
	}

	if (width <= 0) throw std::invalid_argument("Invalid width value");

	m_columns[index].fixedWidth = width;
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

	m_rows[row].values[column] = text;
	m_isDirty = true;
}

void ck::Table::setColor(Color c)
{
	m_color = c;
	m_isDirty = true;
}

void ck::Table::setTheme(Theme theme)
{
	m_theme = theme;
	m_isDirty = true;
}

void ck::Table::clear()
{
	m_rows.clear();
	m_isDirty = true;
}

void ck::Table::clearAlternatingColors()
{
	m_altColors.even = std::nullopt;
	m_altColors.odd = std::nullopt;
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

	bool isAscii = m_theme == Theme::Ascii;
	std::string upperLine = isAscii
		? drawLine(widths)
		: drawUpperLineUnicode(widths);
	
	std::string intermediateLine = isAscii
		? upperLine
		: drawIntermediateLineUnicode(widths);

	std::string lowerLine = isAscii
		? upperLine
		: drawLowerLineUnicode(widths);

	size_t n = m_rows.size();

	output += tColor + upperLine + "\n";
	if (m_visibleHeader) {
		auto view = m_columns
			| std::views::transform([](const Column& c) { return c.value; });
		std::vector<std::string> columnValues(view.begin(), view.end());

		output += tColor;
		output += isAscii 
			? drawRow(columnValues, widths) 
			: drawRowUnicode(columnValues, widths);
		output += "\n";
		output += tColor + (n > 0 ? intermediateLine : lowerLine) + "\n";
	}
	
	for (size_t i = 0; i < n; ++i) {
		std::string rowColor = tColor;
		if (m_rows[i].color.has_value()) {
			rowColor = detail::color_to_ansi(m_rows[i].color.value());
		}
		else if (m_altColors.odd.has_value() && m_altColors.even.has_value()) {
			Color c = (i % 2 == 0) ? m_altColors.even.value() : m_altColors.odd.value();
			rowColor = detail::color_to_ansi(c);
		}

		output += rowColor;
		output += isAscii 
			? drawRow(m_rows[i].values, widths) 
			: drawRowUnicode(m_rows[i].values, widths);
		output += "\n";

		if (i != n - 1) {
			output += tColor + intermediateLine + "\n";
		}
	}
	
	if (!m_rows.empty()) {
		output += tColor + lowerLine;
	}
	
	output += detail::RESET;
	m_cachedOutput = output;
	m_isDirty = false;
	return output + ctx.apply();
}

int ck::Table::getHeight() const
{
	int n = m_rows.size();
	int height = 1; 

	if (m_visibleHeader) height += 2;

	if (n > 0) {
		height += n;       
		height += n - 1;   
		height += 1;       
	}

	return height;
}

std::vector<int> ck::Table::getColumnsWidth() const
{
	std::vector<int> widths;
	for (size_t col = 0; col < m_columns.size(); ++col) {
		int fixedValue = m_columns[col].fixedWidth;
		if (fixedValue != 0) {
			widths.push_back(fixedValue);
			continue;
		}

		int w = detail::visible_length(m_columns[col].value);
		for (const auto& row : m_rows) {
			w = std::max(w, detail::visible_length(row.values[col]));
		}

		widths.push_back(w);
	}

	return widths;
}

std::string ck::Table::pad(const std::string& str, int width, ColumnAlign align) const
{
	int len = detail::visible_length(str);
	int total = width - len;
	if (total <= 0) return str;
	
	if (align == ColumnAlign::Left) {
		return str + std::string(total, ' ');
	}
	else if (align == ColumnAlign::Right) {
		return std::string(total, ' ') + str;
	}
	else {
		int left = total / 2;
		int right = total - left;
		return std::string(left, ' ') + str + std::string(right, ' ');
	}
}

std::string ck::Table::drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const
{
	std::string output;
	output += ASCII_VERTICAL;
	for (size_t i = 0; i < cells.size(); ++i) {
		output += ' ';
		output += pad(cells[i], widths[i], m_columns[i].align);
		output += ' ';
		output += ASCII_VERTICAL;
	}
	return output;
}

std::string ck::Table::drawRowUnicode(const std::vector<std::string>& cells, const std::vector<int>& widths) const
{
	std::string output = UNICODE_VERTICAL;
	for (size_t i = 0; i < cells.size(); ++i) {
		output += ' ';
		output += pad(cells[i], widths[i], m_columns[i].align);
		output += ' ';
		output += UNICODE_VERTICAL;
	}
	return output;
}

std::string ck::Table::drawLine(const std::vector<int>& widths) const
{
	std::string output;
	output += ASCII_CROSS;
	for (size_t i = 0; i < widths.size(); ++i) {
		output += std::string(widths[i] + 2, ASCII_HORIZONTAL);
		output += ASCII_CROSS;
	}
	return output;
}

std::string ck::Table::drawLineUnicode(
	const std::vector<int>& widths,
	const std::string& left_joint,
	const std::string& middle_joint,
	const std::string& right_joint) const
{
	std::string output = left_joint;
	size_t n = widths.size();
	for (size_t i = 0; i < n; ++i) {
		for (int j = 0; j < widths[i] + 2; ++j) output += UNICODE_HORIZONTAL;
		output += (i == n - 1) ? right_joint : middle_joint;
	}
	return output;
}

std::string ck::Table::drawUpperLineUnicode(const std::vector<int>& widths) const
{
	return drawLineUnicode(widths, UNICODE_TOP_LEFT, UNICODE_TOP_T, UNICODE_TOP_RIGHT);
}

std::string ck::Table::drawIntermediateLineUnicode(const std::vector<int>& widths) const
{
	return drawLineUnicode(widths, UNICODE_LEFT_T, UNICODE_CROSS, UNICODE_RIGHT_T);
}

std::string ck::Table::drawLowerLineUnicode(const std::vector<int>& widths) const
{
	return drawLineUnicode(widths, UNICODE_BOTTOM_LEFT, UNICODE_BOTTOM_T, UNICODE_BOTTOM_RIGHT);
}
