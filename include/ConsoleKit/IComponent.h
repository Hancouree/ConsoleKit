#pragma once
#include "Common.h"

namespace ck {
    class ScreenManager;

    class IComponent {
    public:
        virtual ~IComponent() = default;
        virtual void setScreenManager(ScreenManager* manager);
        virtual void setColor(Color c) { m_color = c; }
        virtual Color getColor() const { return m_color; }
        virtual std::string draw(const StyleContext& ctx = {}) const = 0;
        virtual int getHeight() const { return 1; };
    protected:
        ScreenManager* m_mgr = nullptr;
        Color m_color = Grey;
    };
}