#include "../include/ConsoleKit/ScreenManager.h"
#include "../include/ConsoleKit/VLayout.h"
#include <string>

namespace ck {
    ScreenManager::ScreenManager()
        : m_finished(false)
        , m_maxLogs(25)
        , m_rootLayout(std::make_unique<VLayout>())
        , m_lastHeight(0)
    {
        m_rootLayout->setScreenManager(this);
        std::cout << HIDE_CURSOR;
    }

    ScreenManager::~ScreenManager() {
        if (!m_finished) {
            if (m_lastHeight > 0) {
                std::cout << move_down(m_lastHeight);
            }

            std::cout << SHOW_CURSOR << std::endl;
            m_finished = true;
        }
    }

    void ScreenManager::setMaxLogs(size_t n)
    {
        m_maxLogs = n;
    }

    void ScreenManager::refresh()
    {
        if (m_lastHeight > 0) {
            std::cout << move_up(m_lastHeight);
        }

        std::string frameContent = m_rootLayout->draw();
        std::string output;
        int currentHeight = 0;

        std::stringstream ss(frameContent);
        std::string line;
        while (std::getline(ss, line)) {
            output += CLEAR_LINE + line + "\n";
            currentHeight++;
        }

        for (const auto& msg : m_logs) {
            output += CLEAR_LINE + msg + "\n";
            currentHeight++;
        }

        if (currentHeight < m_lastHeight) {
            for (int i = 0; i < (m_lastHeight - currentHeight); ++i) {
                output += CLEAR_LINE + '\n';
            }

            output += move_up(m_lastHeight - currentHeight);
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