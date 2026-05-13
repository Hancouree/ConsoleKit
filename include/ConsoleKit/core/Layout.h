#pragma once
#include "Container.h"

namespace ck {
	class Layout : public Container
	{
	public:
		Layout(Container* parent = nullptr);
		virtual ~Layout() = default;

		void setSpacing(int spacing);
		int getSpacing() const { return m_spacing; }
		virtual int getHeight() const override = 0;

		virtual std::string draw(const StyleContext& ctx = {}) const = 0;
	protected:
		int m_spacing;
	};
}

