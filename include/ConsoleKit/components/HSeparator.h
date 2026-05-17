#pragma once
#include "../core/Component.h"

namespace ck {
	class HSeparator final : public StyledComponent
	{
	public:
		HSeparator(const std::string& label = "", Container* parent = nullptr);

		void setLabel(const std::string& label);
		void setWidth(int width);
		void setChar(char c);
		
		virtual std::string draw(const StyleContext& ctx = {}) const;
	private:
		std::string m_label;
		int m_width;
		char m_char;

		mutable std::string m_cachedOutput;
		mutable bool m_isDirty;

		static constexpr int DEFAULT_WIDTH = 40;
	};
}

