#pragma once
#include <string>
#include <vector>
#include "../core/Container.h"

namespace ck {
	class Panel final : public StyledContainer
	{
	public:
		enum class TitleAlign { Left, Center, Right };

		Panel(const std::string& title = "", Container* parent = nullptr);

		void setTitle(const std::string& title);
		void setHeader(const std::string& text);
		void setFooter(const std::string& text);
		void setTitleAlign(TitleAlign align);
		void setPadding(int horizontal, int vertical);
		void clearContent();

		std::string draw(const StyleContext& ctx = {}) const override;
		int getHeight() const override;
	private:
		void addChild(Component* component) override;

		TitleAlign m_titleAlign;
		int m_horizontalPadding;
		int m_verticalPadding;
		std::string m_title;
		std::vector<std::string> m_headerLines;
		std::vector<std::string> m_footerLines;
	
		static constexpr const char* ASCII_CORNER = "+";
		static constexpr const char* ASCII_HORIZONTAL = "-";
		static constexpr const char* ASCII_VERTICAL = "|";

		static constexpr const char* UNICODE_TOP_LEFT = "╭";
		static constexpr const char* UNICODE_TOP_RIGHT = "╮";
		static constexpr const char* UNICODE_BOTTOM_LEFT = "╰";
		static constexpr const char* UNICODE_BOTTOM_RIGHT = "╯";
		static constexpr const char* UNICODE_HORIZONTAL = "─";
		static constexpr const char* UNICODE_VERTICAL = "│";
	};
}
