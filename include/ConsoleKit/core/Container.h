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

		void setScreenManager(ScreenManager* mgr);
	protected:
		friend class Component;

		virtual void addChild(Component* component);
		Component* removeChild(Component* component);

		std::vector<Component*> m_components;
	};

	class StyledContainer : public Container {
	public:
		StyledContainer(Container* parent);
		virtual ~StyledContainer() = default;

		virtual void setColor(Color c) { m_color = c; }
		virtual Color getColor() const { return m_color; }
	protected:
		Color m_color;
	};
}

