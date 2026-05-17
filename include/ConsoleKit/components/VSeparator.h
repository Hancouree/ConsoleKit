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
		void setChar(char c);

		std::string draw(const StyleContext& ctx = {}) const;
		int getHeight() const override { return m_height; }
	private:
		std::vector<char> getLines(const std::string& raw) const;

		std::string m_label;
		int m_height;
		char m_char;

		mutable std::string m_cachedOutput;
		mutable bool m_isDirty;

		static constexpr int DEFAULT_HEIGHT = 20;
	};
}

