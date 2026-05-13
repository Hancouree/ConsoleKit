#pragma once
#include <string>
#include <vector>
#include "../core/Container.h"

namespace ck {
	class Panel final : public StyledContainer
	{
	public:
		Panel(const std::string& title = "", Container* parent = nullptr);

		void setTitle(const std::string& title);
		void setText(const std::string& text);
		void clearContent();

		std::string draw(const StyleContext& ctx = {}) const override;
		int getHeight() const override;
	private:
		void addChild(Component* component) override;

		std::string m_title;
		std::vector<std::string> m_contentLines;
	};
}
