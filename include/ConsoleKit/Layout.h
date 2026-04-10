#pragma once
#include "IComponent.h"
#include "Container.h"

namespace ck {
    class Layout : public IComponent, public Container {
    public:
        virtual ~Layout() = default;

        template<typename T, typename... Args>
        T& add(Args&&... args) {
            static_assert(std::is_base_of_v<IComponent, T>, "T must inherit from IComponent");
            static_assert(std::is_constructible_v<T, Args...>, "T cannot be constructed with the given arguments");
            auto owner = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *owner;
            ref.setScreenManager(m_mgr);
            m_components.push_back(std::move(owner));
            return ref;
        }

        virtual std::string draw(const StyleContext& ctx = {}) const = 0;
        int getHeight() const override;
    };
}