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

        Layout* getLayout() const;

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