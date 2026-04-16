#include "../include/ConsoleKit/Layout.h"

namespace ck {
    Layout::Layout(Container* parent) 
        : Container(parent) 
        , m_spacing(1) 
    {}

    void Layout::setSpacing(int spacing)
    {
        if (spacing < 0) {
            throw std::invalid_argument("Invalid spacing value");
        }

        m_spacing = spacing;
    }
}