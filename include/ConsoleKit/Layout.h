#pragma once
#include "Component.h"
#include "Container.h"

namespace ck {
    class Layout : public Container {
    public:
        Layout(Container* parent = nullptr);
        virtual ~Layout() = default;
        void setSpacing(int spacing);
        virtual std::string draw(const StyleContext& ctx = {}) const = 0;
    protected:
        int m_spacing;
    };
}