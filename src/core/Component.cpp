#include "../../include/ConsoleKit/core/Component.h"
#include "../../include/ConsoleKit/core/Container.h"

ck::Component::Component(Container* parent) 
    : m_mgr(nullptr)
    , m_parent(nullptr)
    , m_isDirty(true)
{
	setParent(parent);
}

ck::Component::~Component()
{
	if (m_parent) {
		auto* p = m_parent;
		m_parent = nullptr;
		p->removeChild(this);
	}
}

void ck::Component::setParent(Container* parent)
{
    if (m_parent) {
        auto* old = m_parent;
        m_parent = nullptr;
        old->removeChild(this);
    }

    m_parent = parent;

    if (m_parent) {
        m_parent->addChild(this);
        m_mgr = m_parent->getScreenManager();
    }
    else {
        m_mgr = nullptr;
    }
}

ck::StyledComponent::StyledComponent(Container* parent) 
    : Component(parent)
    , m_color(Color::Grey)
    , m_theme(Theme::Ascii)
{
}