#include "../../include/ConsoleKit/core/ScreenManager.h"
#include "../../include/ConsoleKit/core/Layout.h"
#include "../../include/ConsoleKit/core/Common.h"
#include "../../include/ConsoleKit/layouts/VLayout.h"
#include <stdexcept>
#include <Windows.h>
#include <sstream>

ck::ScreenManager::ScreenManager() 
	: m_rootLayout(std::make_unique<VLayout>())
	, m_lastHeight(0)
{
	SetConsoleOutputCP(CP_UTF8);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}

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

	if (currentHeight < m_lastHeight) {
		for (int i = 0; i < m_lastHeight - currentHeight; ++i) {
			output += std::string(detail::CLEAR_LINE) + "\n";
		}

		output += detail::move_up(m_lastHeight - currentHeight);
	}

	std::cout << output << std::flush;
	m_lastHeight = currentHeight;
}
