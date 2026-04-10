#pragma once
#include <memory>
#include <vector>
#include <deque>
#include "Common.h"
#include "Layout.h"

namespace ck {
    class ScreenManager {
    public:
        ScreenManager();
        ~ScreenManager();

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            return m_rootLayout->add<T>(std::forward<Args>(args)...);
        };

        /*ProgressBar& addProgressBar(int finalValue);
        ProgressBar& addProgressBar(int currentValue, int finalValue);
        Spinner& addSpinner(const std::string& str = "");
        ActivityBar& addActivityBar(const std::string& str = "");
        Table& addTable(const std::vector<std::string>& columns);
        Panel& addPanel(const std::string& title = "");*/

        void setMaxLogs(size_t n);
        void refresh();
        void log(const std::string& message);
    private:
        std::unique_ptr<Layout> m_rootLayout;
        std::deque<std::string> m_logs;
        size_t m_maxLogs;
        bool m_finished;
        int m_lastHeight;
    };
}