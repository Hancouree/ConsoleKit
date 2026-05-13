#include "../../include/ConsoleKit/layouts/VLayout.h"

ck::VLayout::VLayout(Container* parent) : Layout(parent)
{
}

std::string ck::VLayout::draw(const StyleContext& ctx) const
{
    std::string output;
    bool first = true;
    for (auto* c : m_components) {
        if (!c) continue;
        if (!first) output += std::string(m_spacing + 1, '\n');
        
        output += c->draw(ctx);
        first = false;
    }

    return output;
}

int ck::VLayout::getHeight() const
{
    if (m_components.empty()) return 0;
    int totalHeight = 0;
    for (auto* c : m_components) {
        totalHeight += c->getHeight();
    }
    return totalHeight + (static_cast<int>(m_components.size()) - 1) * m_spacing;
}
