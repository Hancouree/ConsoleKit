#pragma once
#include <string>
#include "../core/Layout.h"

namespace ck {
	class VLayout final : public Layout
	{
	public:
		VLayout(Container* parent = nullptr);
		std::string draw(const StyleContext& ctx = {}) const override;
		int getHeight() const override;
	};
}

