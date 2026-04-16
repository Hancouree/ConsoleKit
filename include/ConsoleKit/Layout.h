#pragma once
#include "Component.h"
#include "Container.h"

namespace ck {
    class Layout : public Container {
    public:
        Layout(Container* parent = nullptr);
        virtual ~Layout() = default;

        virtual std::string draw(const StyleContext& ctx = {}) const = 0;
        int getHeight() const override;
    };
}