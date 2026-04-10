#pragma once
#include "Layout.h"

namespace ck {
    class VLayout final : public Layout {
    public:
        std::string draw(const StyleContext& ctx = {}) const;
    };
}