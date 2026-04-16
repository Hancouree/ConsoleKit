#pragma once
#include "../Layout.h"

namespace ck {
	class HLayout final : public Layout
	{
	public:
		HLayout(Container* parent = nullptr);
		int getHeight() const override;
		std::string draw(const StyleContext& ctx = {}) const;
	};
}

