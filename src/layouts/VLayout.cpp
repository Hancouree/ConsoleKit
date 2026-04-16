#include "../../include/ConsoleKit/layouts/VLayout.h"

namespace ck {
    VLayout::VLayout(Container* parent) : Layout(parent) {}

    std::string VLayout::draw(const StyleContext& ctx) const {
        std::string output;
        for (size_t i = 0; i < m_components.size(); ++i) {
            if (!m_components[i]) continue;
            output += m_components[i]->draw(ctx);
            if (i < m_components.size() - 1) {
                output += "\n";
            }
        }
        return output;
    }
}