#include "../include/ConsoleKit/Layout.h"
#include <numeric>

namespace ck {
    Layout::Layout(Container* parent) : Container(parent) {}

    int Layout::getHeight() const {
        return std::accumulate(m_components.begin(), m_components.end(), 0,
            [](int x, Component* c) {
                return x + (c ? c->getHeight() : 0);
            });
    }
}