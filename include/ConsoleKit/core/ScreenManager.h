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
	private:
		std::unique_ptr<Layout> m_rootLayout;
		int m_lastHeight;
	};
}

