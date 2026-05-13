#include "../../include/ConsoleKit/core/ScreenManager.h"
#include "../../include/ConsoleKit/core/Layout.h"
#include "../../include/ConsoleKit/core/Common.h"
#include "../../include/ConsoleKit/layouts/VLayout.h"
#include <stdexcept>
#include <sstream>

ck::ScreenManager::ScreenManager() 
	: m_rootLayout(std::make_unique<VLayout>())
	, m_maxLogs(25)
	, m_lastHeight(0)
{
	m_rootLayout->setScreenManager(this);
	std::cout << detail::HIDE_CURSOR;
}

ck::ScreenManager::~ScreenManager()
{
	std::cout << detail::SHOW_CURSOR << std::endl;
}

void ck::ScreenManager::setLayout(Layout* layout)
{
	if (!layout) return;
	m_rootLayout.reset(layout);
	m_rootLayout->setScreenManager(this);
}

void ck::ScreenManager::tick()
{
	m_rootLayout->tick();
}

void ck::ScreenManager::refresh()
{
	if (m_lastHeight > 0) {
		std::cout << detail::move_up(m_lastHeight);
	}

	std::string frameContext = m_rootLayout->draw();
	std::string output;
	int currentHeight = 0;

	std::stringstream ss(frameContext);
	std::string line;
	while (std::getline(ss, line)) {
		output += detail::CLEAR_LINE + line + "\n";
		++currentHeight;
	}

	for (const auto& msg : m_logs) {
		output += msg + "\n";
		++currentHeight;
	}

	if (currentHeight < m_lastHeight) {
		for (int i = 0; i < m_lastHeight - currentHeight; ++i) {
			output += std::string(detail::CLEAR_LINE) + "\n";
		}

		output += detail::move_up(m_lastHeight - currentHeight);
	}

	std::cout << output << std::flush;
	m_lastHeight = currentHeight;
}

void ck::ScreenManager::log(const std::string& message)
{
	auto lines = detail::splitLines(message);
	for (auto& l : lines) m_logs.push_back(std::move(l));
	while (m_logs.size() > m_maxLogs) { m_logs.pop_front(); }
	refresh();
}

void ck::ScreenManager::setMaxLogs(size_t n)
{
	m_maxLogs = n;
}
