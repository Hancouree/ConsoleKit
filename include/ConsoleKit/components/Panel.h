#pragma once
#include "../Component.h"
#include "../Container.h"

namespace ck {
    class Panel final : public StyledContainer {
    public:
        Panel(const std::string& title = "", Container* parent = nullptr);

        void setTitle(const std::string& title);
        void setText(const std::string& text);

        virtual void takeComponent(Component* component) override;

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