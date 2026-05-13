#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <list>

namespace ck {
	class Layout;

	class ScreenManager
	{
	public:
		ScreenManager();
		~ScreenManager();

		void setLayout(Layout* layout);
		Layout* getLayout() const { return m_rootLayout.get(); }

		void tick();
		void refresh();

		void log(const std::string& message);
		void setMaxLogs(size_t n);
	private:
		std::unique_ptr<Layout> m_rootLayout;
		std::list<std::string> m_logs;
		size_t m_maxLogs;
		int m_lastHeight;
	};
}

