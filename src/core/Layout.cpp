#include "../../include/ConsoleKit/core/Layout.h"
#include <stdexcept>

ck::Layout::Layout(Container* parent) 
	: Container(parent)
	, m_spacing(1)
{
}

void ck::Layout::setSpacing(int spacing)
{
	if (spacing < 0) throw std::invalid_argument("Invalid spacing value");
	m_spacing = spacing;
}
