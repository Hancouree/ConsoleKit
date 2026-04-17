#include "../../include/ConsoleKit/components/Table.h"
#include "../../include/ConsoleKit/ScreenManager.h"

namespace ck {
    Table::Table(const std::vector<std::string>& columns, Container* parent) 
        : StyledComponent(parent)
        , m_columns(columns)
        , m_showBorders(true)
        , m_wasInfoUpdated(false)
    {
        m_fixedColumnWidths.resize(m_columns.size(), 0);
    }

    void Table::addRow(const std::vector<std::string>& row)
    {
        if (row.size() != m_columns.size()) {
            throw std::invalid_argument("Row size doesn't match column count");
        }
        m_rows.push_back(row);
        m_wasInfoUpdated = true;
    }

    void Table::removeRow(int row)
    {
        if (row < 0 || row >= m_rows.size()) {
            throw std::invalid_argument("Invalid row value");
        }

        m_rows.erase(m_rows.begin() + row);
        m_wasInfoUpdated = true;
    }

    std::string Table::draw(const StyleContext& ctx) const
    {
        if (!m_wasInfoUpdated && !m_cachedOutput.empty()) {
            return m_cachedOutput + ctx.apply();
        }
        else {

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

            m_cachedOutput = output;
            output += ctx.apply();

            return output;
        }
    }

    void Table::setColor(Color c)
    {
        m_color = c;
        m_wasInfoUpdated = true;
    }

    void Table::setColumnWidth(int column, int v)
    {
        if (column < 0 || column >= m_columns.size()) {
            throw std::invalid_argument("Invalid column value");
        }

        m_fixedColumnWidths[column] = v;
    }

    void Table::setCell(int row, int column, const std::string& v)
    {
        if (row < 0 || row >= m_rows.size()) {
            throw std::invalid_argument("Invalid row value");
        }

        if (column < 0 || column >= m_columns.size()) {
            throw std::invalid_argument("Invalid column value");
        }

        m_rows[row][column] = v;
        m_wasInfoUpdated = true;
    }

    void Table::update()
    {
        if (m_wasInfoUpdated) {
            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << "\r" << draw() << std::flush;
            }

            m_wasInfoUpdated = false;
        }
    }

    void Table::clear()
    {
        m_rows.clear();
        m_wasInfoUpdated = true;
    }

    std::vector<int> Table::getColumnsWidth() const
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

    int Table::getHeight() const
    {
        int rowsSize = m_rows.size();
        if (m_showBorders) {
            return rowsSize > 0 ? 4 + rowsSize : 3;
        }
        else {
            return rowsSize + 1;
        }
    }

    Table& Table::withBorders(bool enable)
    {
        m_showBorders = enable;
        return *this;
    }

    std::string Table::pad(const std::string& str, int width) const
    {
        return str + std::string(width - detail::visible_length(str), ' ');
    }

    std::string Table::drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const
    {
        std::string output = m_showBorders ? "|" : "";
        for (size_t i = 0; i < cells.size(); ++i) {
            output += pad(cells[i], widths[i]);
            if (m_showBorders) output += (i + 1 < cells.size()) ? " | " : "|";
            else output += (i + 1 < cells.size()) ? "  " : "";
        }
        return output;
    }

    std::string Table::drawLine(const std::vector<int>& widths, char sep) const
    {
        if (!m_showBorders) return "";

        std::string output = "+";
        for (size_t i = 0; i < widths.size(); ++i) {
            output += std::string(widths[i], '-');
            output += (i + 1 < widths.size()) ? '-' + std::string(1, sep) + '-' : "+";
        }
        return output;
    }
}