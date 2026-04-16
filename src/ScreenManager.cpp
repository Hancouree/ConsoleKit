#include "../include/ConsoleKit/ScreenManager.h"
#include "../include/ConsoleKit/layouts/VLayout.h"
#include <string>
#include <sstream>

namespace ck {
    ScreenManager::ScreenManager()
        : m_finished(false)
        , m_maxLogs(25)
        , m_rootLayout(std::make_unique<VLayout>())
        , m_lastHeight(0)
    {
        m_rootLayout->setScreenManager(this);
        std::cout << detail::HIDE_CURSOR;
    }

    ScreenManager::~ScreenManager() {
        if (!m_finished) {
            if (m_lastHeight > 0) {
                std::cout << detail::move_down(m_lastHeight);
            }

            std::cout << detail::SHOW_CURSOR << std::endl;
            m_finished = true;
        }
    }

    Layout* ScreenManager::getLayout() const
    {
        return m_rootLayout.get();
    }

    void ScreenManager::setMaxLogs(size_t n)
    {
        m_maxLogs = n;
    }

    void ScreenManager::refresh()
    {
        if (m_lastHeight > 0) {
            std::cout << detail::move_up(m_lastHeight);
        }

        std::string frameContent = m_rootLayout->draw();
        std::string output;
        int currentHeight = 0;

        std::stringstream ss(frameContent);
        std::string line;
        while (std::getline(ss, line)) {
            output += detail::CLEAR_LINE + line + "\n";
            currentHeight++;
        }

        for (const auto& msg : m_logs) {
            output += detail::CLEAR_LINE + msg + "\n";
            currentHeight++;
        }

        if (currentHeight < m_lastHeight) {
            for (int i = 0; i < (m_lastHeight - currentHeight); ++i) {
                output += detail::CLEAR_LINE + '\n';
            }

            output += detail::move_up(m_lastHeight - currentHeight);
        }

        std::cout << output << std::flush;
        m_lastHeight = currentHeight;
    }

    void ScreenManager::log(const std::string& message)
    {
        if (m_logs.size() >= m_maxLogs) {
            m_logs.pop_front();
        }

        m_logs.push_back(message);
        refresh();
    }
}