#include "../../include/ConsoleKit/core/Container.h"

ck::Container::Container(Container* parent) : Component(parent)
{
}

ck::Container::~Container()
{
	auto components = std::move(m_components);
	for (auto* c : components) {
		delete c;
	}
}

void ck::Container::tick()
{
	for (auto* c : m_components) { c->tick(); }
}

void ck::Container::setScreenManager(ScreenManager* mgr)
{
	m_mgr = mgr;
	for (auto* c : m_components) {
		c->setScreenManager(mgr);
	}
}

void ck::Container::addChild(Component* component)
{
	if (component) {
		m_components.push_back(component);
		component->setScreenManager(m_mgr);
	}
}

ck::Component* ck::Container::removeChild(Component* component)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it) {
		if (*it == component) {
			m_components.erase(it);
			component->m_parent = nullptr;
			component->setScreenManager(nullptr);
			break;
		}
	}

	return component;
}

ck::StyledContainer::StyledContainer(Container* parent)
	: Container(parent)
	, m_color(Color::Grey)
{
}
