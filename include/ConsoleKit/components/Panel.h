#pragma once
#include "../IComponent.h"
#include "../Container.h"

namespace ck {
    class Panel : public IComponent, public Container {
    public:
        Panel(const std::string& title = "");
        void setTitle(const std::string& title);
        void setText(const std::string& text);

        template <typename T, typename... Args>
        T& setComponent(Args&&... args) {
            static_assert(std::is_base_of_v<IComponent, T>, "T must inherit from IComponent");
            static_assert(std::is_constructible_v<T, Args...>, "T cannot be constructed with the given arguments");
            auto owner = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *owner;

            takeComponent(std::move(owner));

            return ref;
        }

        virtual void takeComponent(std::unique_ptr<IComponent> component) override;

        std::string draw(const StyleContext& ctx = {}) const override;
        void update();
        int getHeight() const override;
    private:
        int m_minUpdateIntervalMs = 100;
        std::string m_title;
        std::vector<std::string> m_contentLines;
        std::chrono::steady_clock::time_point m_lastUpdate;
    };
}