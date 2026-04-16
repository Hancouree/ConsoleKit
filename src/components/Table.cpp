#include "../../include/ConsoleKit/components/Table.h"
#include "../../include/ConsoleKit/ScreenManager.h"

namespace ck {
    Table::Table(const std::vector<std::string>& columns, Container* parent) 
        : StyledComponent(parent)
        , m_wasInfoUpdated(false)
        , m_columns(columns)
    {}

    void Table::addRow(const std::vector<std::string>& row)
    {
        if (row.size() != m_columns.size()) {
            throw std::invalid_argument("Row size doesn't match column count");
        }
        m_rows.push_back(row);
        m_wasInfoUpdated = true;

        update();
    }

    std::string Table::draw(const StyleContext& ctx) const
    {
        auto widths = getColumnsWidth();
        std::string tColor = detail::color_to_ansi(m_color);

        std::string output;

        output += tColor + drawLine(widths, '-') + "\n";
        output += tColor + drawRow(m_columns, widths) + "\n";
        output += tColor + drawLine(widths, '+') + "\n";

        for (const auto& row : m_rows) {
            output += tColor + drawRow(row, widths) + "\n";
        }

        if (!m_rows.empty()) {
            output += tColor + drawLine(widths, '-');
        }

        output += ctx.apply();
        return output;
    }

    void Table::setColor(Color c)
    {
        m_color = c;
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

    std::vector<int> Table::getColumnsWidth() const
    {
        std::vector<int> widths;
        for (size_t col = 0; col < m_columns.size(); ++col) {
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
        return rowsSize > 0 ? 4 + rowsSize : 3;
    }

    std::string Table::pad(const std::string& str, int width) const
    {
        return str + std::string(width - str.size(), ' ');
    }

    std::string Table::drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const
    {
        std::string output = "|";
        for (size_t i = 0; i < cells.size(); ++i) {
            output += pad(cells[i], widths[i]);
            output += (i + 1 < cells.size()) ? " | " : "|";
        }
        return output;
    }

    std::string Table::drawLine(const std::vector<int>& widths, char sep) const
    {
        std::string output = "+";
        for (size_t i = 0; i < widths.size(); ++i) {
            output += std::string(widths[i], '-');
            output += (i + 1 < widths.size()) ? '-' + std::string(1, sep) + '-' : "+";
        }
        return output;
    }
}