#include "../include/ConsoleKit/Component.h"
#include "../include/ConsoleKit/Container.h"

ck::Component::Component(Container* parent) : m_parent(parent)
{
	setParent(parent);
}

ck::Component::~Component()
{
	if (m_parent) {
		m_parent->removeChild(this);
	}
}

void ck::Component::setParent(Container* parent)
{
	if (m_parent) {
		m_parent->removeChild(this);
	}

	m_parent = parent;

	if (m_parent) {
		m_parent->addChild(this);
		m_mgr = m_parent->getScreenManager();
	}
}

ck::StyledComponent::StyledComponent(Container* parent) : Component(parent) {}