#pragma once
#include <vector>
#include "Component.h"
#include "Common.h"

namespace ck {
	class ScreenManager;

	class Container : public Component
	{
	public:
		Container(Container* parent);
		virtual ~Container();

		void tick();
		void setComponentTickInterval(Component* c, int intervalMs);

		void setScreenManager(ScreenManager* mgr);
	protected:
		friend class Component;
		struct ComponentEntry {
			Component* c;
			std::chrono::milliseconds minInterval;
			std::chrono::steady_clock::time_point lastTick;

			bool shouldTick() const {
				auto now = detail::GET_NOW() - lastTick;
				return now >= minInterval;
			}
		};

		virtual void addChild(Component* component);
		Component* removeChild(Component* component);

		std::vector<ComponentEntry> m_components;

		static constexpr int DEFAULT_TICK_INTERVAL = 70;
	};

	class StyledContainer : public Container {
	public:
		StyledContainer(Container* parent);
		virtual ~StyledContainer() = default;

		virtual void setColor(Color c) { m_color = c; }
		virtual void setTheme(Theme theme) { m_theme = theme; }
		virtual Color getColor() const { return m_color; }
		virtual Theme getTheme() const { return m_theme; }
	protected:
		Color m_color;
		Theme m_theme;
	};
}

