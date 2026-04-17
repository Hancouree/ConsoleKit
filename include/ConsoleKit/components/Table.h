#pragma once
#include "../Component.h"

namespace ck {
    class Table : public StyledComponent {
    public:
        Table(const std::vector<std::string>& columns, Container* parent = nullptr);

        void addRow(const std::vector<std::string>& row);
        void removeRow(int row);
        void setColor(Color c) override;
        void setColumnWidth(int column, int v);
        void setCell(int row, int column, const std::string& v);

        std::string draw(const StyleContext& ctx = {}) const override;
        void update();
        void clear();

        std::vector<int> getColumnsWidth() const;
        int getHeight() const override;

        Table& withBorders(bool enable = true);
    private:
        std::string pad(const std::string& str, int width) const;
        std::string drawRow(const std::vector<std::string>& cells, const std::vector<int>& widths) const;
        std::string drawLine(const std::vector<int>& widths, char sep) const;

        int m_minUpdateIntervalMs = 50;
        std::vector<std::string> m_columns;
        std::vector<std::vector<std::string>> m_rows;
        std::vector<int> m_fixedColumnWidths;
        mutable std::string m_cachedOutput;
        bool m_showBorders;
        bool m_wasInfoUpdated;
    };
}