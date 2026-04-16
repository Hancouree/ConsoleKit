#include "../../include/ConsoleKit/layouts/VLayout.h"

namespace ck {
    VLayout::VLayout(Container* parent) : Layout(parent) {}

    std::string VLayout::draw(const StyleContext& ctx) const {
        std::string output;
        bool first = true;
        for (const auto* comp : m_components) {
            if (!comp) continue;

            if (!first) {
                output += std::string(m_spacing + 1, '\n');
            }

            output += comp->draw(ctx);
            first = false;
        }

        return output;
    }

    int VLayout::getHeight() const {
        int activeCount = 0;
        int totalHeight = 0;
        for (auto* c : m_components) {
            if (c) {
                totalHeight += c->getHeight();
                activeCount++;
            }
        }
        return totalHeight + (activeCount - 1) * m_spacing;
    }
}