#include "../include/ConsoleKit/Container.h"

namespace ck {
    Container::Container(Container* parent) : Component(parent) {}

    Container::~Container()
    {
        for (auto* c : m_components) {
            if (c) delete c;
        }

        m_components.clear();
    }

    void Container::takeComponent(Component* component)
    {
        if (component) m_components.push_back(component);
    }

    Component* Container::releaseComponent(Component* component)
    {
        for (auto it = m_components.begin(); it != m_components.end(); ++it) {
            if (*it == component) {
                m_components.erase(it);
                break;
            }
        }

        return component;
    }

    StyledContainer::StyledContainer(Container* parent) : Container(parent) {}
}