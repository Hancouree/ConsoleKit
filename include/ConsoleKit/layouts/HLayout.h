#pragma once
#include <string>
#include "../core/Layout.h"

namespace ck {
	class HLayout final : public Layout
	{
	public:
		HLayout(Container* parent = nullptr);
		std::string draw(const StyleContext& ctx = {}) const override;
		int getHeight() const override;
	};
}

