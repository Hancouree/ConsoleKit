#include "../include/ConsoleKit/Container.h"

namespace ck {
    void Container::takeComponent(std::unique_ptr<IComponent> component)
    {
        if (component)
            m_components.push_back(std::move(component));
    }

    std::unique_ptr<IComponent> Container::releaseComponent(IComponent* component)
    {
        for (auto it = m_components.begin(); it != m_components.end(); ++it) {
            if (it->get() == component) {
                auto component = std::move(*it);
                m_components.erase(it);
                return component;
            }
        }

        return nullptr;
    }
}