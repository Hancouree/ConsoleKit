#include "../../include/ConsoleKit/core/Container.h"

ck::Container::Container(Container* parent) : Component(parent)
{
}

ck::Container::~Container()
{
	auto components = std::move(m_components);
	for (const auto& e : components) {
		delete e.c;
	}
}

void ck::Container::tick()
{
	for (auto& e : m_components) { 
		if (e.shouldTick()) {
			e.c->tick();
			e.lastTick = detail::GET_NOW();
		}
	}
}

void ck::Container::setComponentTickInterval(Component* c, int intervalMs)
{
	if (intervalMs < 0) throw std::invalid_argument("Invalid interval value");
	for (auto& e : m_components) {
		if (e.c == c) {
			e.minInterval = std::chrono::milliseconds(intervalMs);
		}
	}
}

void ck::Container::setScreenManager(ScreenManager* mgr)
{
	m_mgr = mgr;
	for (const auto& e : m_components) {
		e.c->setScreenManager(mgr);
	}
}

void ck::Container::addChild(Component* component)
{
	if (component) {
		auto interval = std::chrono::milliseconds(DEFAULT_TICK_INTERVAL);
		m_components.push_back({ component, interval, detail::GET_NOW() - interval });
		component->setScreenManager(m_mgr);
	}
}

ck::Component* ck::Container::removeChild(Component* component)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it) {
		if (it->c == component) {
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
	, m_theme(Theme::Ascii)
{
}
