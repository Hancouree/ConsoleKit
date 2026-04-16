#include "../../include/ConsoleKit/components/Panel.h"
#include "../../include/ConsoleKit/ScreenManager.h"

namespace ck {
    Panel::Panel(const std::string& title, Container* parent) 
        : StyledContainer(parent)
        , m_title(title)
    {
    }

    void Panel::setTitle(const std::string& title)
    {
        m_title = title;
    }

    void Panel::setText(const std::string& text)
    {
        m_contentLines = detail::splitLines(text);
        update();
    }

    void Panel::takeComponent(Component* component)
    {
        if (!m_components.empty()) {
            throw std::logic_error(
                "Panel error: Attempted to set a component, but the panel already has one. "
                "Clear or release the existing component first."
            );
        }

        Container::takeComponent(component);
    }

    std::string Panel::draw(const StyleContext& ctx) const
    {
        StyleContext innerCtx = ctx;
        if (m_color != Grey) innerCtx.fg = m_color;

        std::string pColor = detail::color_to_ansi(m_color);
        int titleVisLen = detail::visible_length(m_title);

        std::vector<std::string> lines;
        if (!m_components.empty()) {
            lines = detail::splitLines(m_components[0]->draw(innerCtx));
        }
        else {
            lines = m_contentLines;
        }

        if (lines.empty() && m_title.empty()) return "";

        int maxWidth = titleVisLen;
        for (const auto& l : lines) maxWidth = std::max(maxWidth, detail::visible_length(l));
        int innerWidth = static_cast<int>(maxWidth) + 2;

        std::string output = pColor;

        output += "+-";
        if (!m_title.empty()) {
            output += m_title + " ";
            int fill = innerWidth - static_cast<int>(titleVisLen) - 2;
            if (fill > 0) output += std::string(fill, '-');
        }
        else {
            output += std::string(innerWidth - 1, '-');
        }
        output += "+\n";

        for (const auto& line : lines) {
            output += pColor + "| " + detail::RESET;
            output += line;
            output += pColor + std::string(std::max(0, innerWidth - static_cast<int>(detail::visible_length(line)) - 1), ' ') + "|\n";
        }

        output += pColor + "+" + std::string(innerWidth, '-') + "+";

        output += ctx.apply();

        return output;
    }

    void Panel::update()
    {
        auto now = detail::GET_NOW();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastUpdate).count();

        if (elapsed > m_minUpdateIntervalMs) {
            if (m_mgr) {
                m_mgr->refresh();
            }
            else {
                std::cout << "\r" << draw() << std::flush;
            }

            m_lastUpdate = now;
        }
    }

    int Panel::getHeight() const
    {
        int contentHeight = !m_components.empty() ? m_components[0]->getHeight() : m_contentLines.size();
        return contentHeight + 2;
    }
}