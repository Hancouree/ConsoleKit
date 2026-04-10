#include "../include/ConsoleKit/IComponent.h"
#include "../include/ConsoleKit/ScreenManager.h"

void ck::IComponent::setScreenManager(ScreenManager* manager)
{
	m_mgr = manager;
}