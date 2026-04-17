#pragma once
#include <memory>
#include "Component.h"

namespace ck {
    class Container : public Component {
    public:
        Container(Container* parent = nullptr);
        virtual ~Container();
    protected:
        friend class Component;

        virtual void addChild(Component* component);
        Component* removeChild(Component* component);

        std::vector<Component*> m_components;
    };

    class StyledContainer : public Container {
    public:
        StyledContainer(Container* parent = nullptr);
        virtual ~StyledContainer() = default;

        virtual void setColor(Color c) { m_color = c; }
        virtual Color getColor() const { return m_color; }
    protected:
        Color m_color = Grey;
    };
}