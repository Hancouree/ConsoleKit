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

		mutable std::string m_cachedOutput;
		mutable bool m_isDirty;
	};

	class StyledComponent : public Component {
	public:
		StyledComponent(Container* parent = nullptr);
		virtual ~StyledComponent() = default;

		virtual void setColor(Color c) { m_color = c; }
		virtual void setTheme(Theme theme) { m_theme = theme; }
		virtual Color getColor() const { return m_color; }
		virtual Theme getTheme() const { return m_theme; }
	protected:
		Color m_color;
		Theme m_theme;
	};
}

