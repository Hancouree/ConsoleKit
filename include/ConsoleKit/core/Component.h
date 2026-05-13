#pragma once
#include <string>
#include "Common.h"

namespace ck {
	class ScreenManager;
	class Container;

	class Component
	{
	public:
		Component(Container* parent = nullptr);
		virtual ~Component();

		virtual void tick() {};
		virtual std::string draw(const StyleContext& ctx = {}) const = 0;
		virtual int getHeight() const { return 1; }

		virtual void setParent(Container* parent);
		virtual void setScreenManager(ScreenManager* mgr) { m_mgr = mgr; }
		ScreenManager* getScreenManager() const { return m_mgr; }
	protected:
		friend class Container;

		ScreenManager* m_mgr;
		Container* m_parent;
	};

	class StyledComponent : public Component {
	public:
		StyledComponent(Container* parent = nullptr);
		virtual ~StyledComponent() = default;

		virtual void setColor(Color c) { m_color = c; }
		virtual Color getColor() const { return m_color; }
	protected:
		Color m_color;
	};
}

