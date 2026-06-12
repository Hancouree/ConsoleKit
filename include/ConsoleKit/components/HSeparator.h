#pragma once
#include "../core/Component.h"
#include "../core/Common.h"

namespace ck {
	class HSeparator final : public StyledComponent
	{
	public:
		HSeparator(const std::string& label = "", Container* parent = nullptr);

		void setLabel(const std::string& label);
		void setWidth(int width);
		void setTheme(Theme theme) override;

		virtual std::string draw(const StyleContext& ctx = {}) const;
	private:
		std::string m_label;
		int m_width;

		static constexpr int DEFAULT_WIDTH = 30;
		static constexpr const char* ASCII_SYMBOL = "-";
		static constexpr const char* UNICODE_SYMBOL = "━";
	};
}

