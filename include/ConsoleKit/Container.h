#pragma once
#include <memory>
#include "Component.h"

namespace ck {
    class Container : public Component {
    public:
        Container(Container* parent = nullptr);
        virtual ~Container();

        virtual void takeComponent(Component* component);
        Component* releaseComponent(Component* component);
    protected:
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