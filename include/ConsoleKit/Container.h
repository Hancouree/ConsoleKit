#pragma once
#include <memory>
#include "IComponent.h"

namespace ck {
    class Container {
    public:
        virtual ~Container() = default;
        virtual void takeComponent(std::unique_ptr<IComponent> component);
        std::unique_ptr<IComponent> releaseComponent(IComponent* component);
    protected:
        std::vector<std::unique_ptr<IComponent>> m_components;
    };
}