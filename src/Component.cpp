#include "../include/ConsoleKit/Component.h"
#include "../include/ConsoleKit/Container.h"

ck::Component::Component(Container* parent) : m_parent(parent)
{
	setParent(parent);
}

ck::Component::~Component()
{
	if (m_parent) {
		m_parent->releaseComponent(this);
	}
}

void ck::Component::setParent(Container* parent)
{
	if (m_parent) {
		m_parent->releaseComponent(this);
	}

	m_parent = parent;

	if (m_parent) {
		m_parent->takeComponent(this);
		m_mgr = m_parent->getScreenManager();
	}
}

ck::StyledComponent::StyledComponent(Container* parent) : Component(parent) {}