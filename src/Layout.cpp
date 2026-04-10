#include "../include/ConsoleKit/Layout.h"
#include <numeric>

namespace ck {
    int Layout::getHeight() const {
        return std::accumulate(m_components.begin(), m_components.end(), 0,
            [](int x, const std::unique_ptr<IComponent>& c) {
                return x + (c ? c->getHeight() : 0);
            });
    }
}