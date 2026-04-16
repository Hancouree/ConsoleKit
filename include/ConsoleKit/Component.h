#pragma once
#include "Common.h"

namespace ck {
    class ScreenManager;
    class Container;

    class Component {
    public:
        Component(Container* parent = nullptr);
        virtual ~Component();

        void setScreenManager(ScreenManager* mgr) { m_mgr = mgr; }
        virtual void setParent(Container* manager);

        virtual std::string draw(const StyleContext& ctx = {}) const = 0;

        ScreenManager* getScreenManager() const { return m_mgr; }
        virtual int getHeight() const { return 1; };
    protected:
        ScreenManager* m_mgr = nullptr;
        Container* m_parent = nullptr;
    };

    class StyledComponent : public Component
    {
    public:
        StyledComponent(Container* parent = nullptr);
        virtual ~StyledComponent() = default;

        virtual void setColor(Color c) { m_color = c; }
        virtual Color getColor() const { return m_color; }
    protected:
        Color m_color = Grey;
    };
}