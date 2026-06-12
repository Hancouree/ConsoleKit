#pragma once
#include <string>
#include <vector>
#include "../core/Component.h"

namespace ck {
	class VSeparator final : public StyledComponent
	{
	public:
		VSeparator(const std::string& label = "", Container* parent = nullptr);
		
		void setLabel(const std::string& label);
		void setHeight(int height);
		void setTheme(Theme theme) override;

		std::string draw(const StyleContext& ctx = {}) const;
		int getHeight() const override { return m_height; }
	private:
		std::vector<std::string> getLines(const std::string& raw) const;

		std::string m_label;
		int m_height;

		static constexpr int DEFAULT_HEIGHT = 15;
		static constexpr const char* ASCII_SYMBOL = "|";
		static constexpr char SPACE = ' ';
		static constexpr const char* UNICODE_SYMBOL = "┃";
	};
}

