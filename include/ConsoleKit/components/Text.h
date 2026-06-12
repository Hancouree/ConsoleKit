#pragma once
#include <string>
#include <vector>
#include "../core/Component.h"

namespace ck {
	class Text final : public StyledComponent
	{
	public:
		enum class Align { Left, Center, Right };
		enum class Overflow { Wrap, Truncate };

		Text(const std::string& value, Container* parent = nullptr);
		
		void setText(const std::string& value);
		void setAlign(Align align);
		void setOverflow(Overflow overflow);
		void setMaxWidth(int width);

		std::string draw(const StyleContext& ctx = {}) const;
		int getHeight() const { return m_lastHeight; }
		int getWidth() const { return m_maxWidth; };
	private:
		std::vector<std::string> getLines(std::string raw) const;
		void setTheme(Theme theme) override {};

		std::string m_rawText;
		Align m_align;
		Overflow m_overflow;
		int m_maxWidth;
	
		mutable std::string m_cachedOutput;
		mutable int m_lastHeight;
		mutable bool m_isDirty;

		static constexpr int DEFAULT_MAX_WIDTH = 50;
	};
}

