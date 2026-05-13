#include "../../include/ConsoleKit/components/Panel.h"
#include "../../include/ConsoleKit/core/Common.h"

ck::Panel::Panel(const std::string& title, Container* parent)
	: StyledContainer(parent),
	m_title(title)
{
}

void ck::Panel::setTitle(const std::string& title)
{
	m_title = title;
}

void ck::Panel::setText(const std::string& text)
{
	m_contentLines = detail::splitLines(text);
}

void ck::Panel::clearContent()
{
	m_contentLines.clear();
	if (!m_components.empty()) {
		auto* child = removeChild(m_components.front());
		delete child;
	}
}

std::string ck::Panel::draw(const StyleContext& ctx) const
{
    StyleContext innerCtx = ctx;
    innerCtx.fg = m_color;

    std::string pColor = detail::color_to_ansi(m_color);
    int titleVisLen = detail::visible_length(m_title);

    std::vector<std::string> lines;
    for (const auto& l : m_contentLines) {
        lines.push_back(l);
    }
    if (!m_components.empty()) {
        for (auto& l : detail::splitLines(m_components[0]->draw(innerCtx))) {
            lines.push_back(std::move(l));
        }
    }

    if (lines.empty() && m_title.empty()) return "";

    int maxWidth = titleVisLen;
    for (const auto& l : lines) {
        maxWidth = std::max(maxWidth, detail::visible_length(l));
    }
    int innerWidth = maxWidth + 2;

    std::string output = pColor;
    output += "+-";
    if (!m_title.empty()) {
        output += m_title + " ";
        int fill = innerWidth - titleVisLen - 2;
        if (fill > 0) output += std::string(fill, '-');
    }
    else {
        output += std::string(innerWidth - 1, '-');
    }
    output += "+\n";

    for (const auto& line : lines) {
        output += pColor + "| " + detail::RESET;
        output += line;
        int pad = innerWidth - detail::visible_length(line) - 1;
        output += pColor + std::string(std::max(0, pad), ' ') + "|\n";
    }

    output += pColor + "+" + std::string(innerWidth, '-') + "+";
    output += ctx.apply();
    return output;
}

int ck::Panel::getHeight() const
{
    int contentHeight = m_contentLines.size();
    if (!m_components.empty()) {
        contentHeight += m_components[0]->getHeight();
    }
    return contentHeight + 2;
}

void ck::Panel::addChild(Component* component)
{
	if (!m_components.empty()) {
		throw std::logic_error(
			"Panel already has a component, call clearContent() first"
		);
	}
	Container::addChild(component);
}
