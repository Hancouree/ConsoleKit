#include "../../include/ConsoleKit/components/Panel.h"
#include "../../include/ConsoleKit/core/Common.h"
#include <stdexcept>

ck::Panel::Panel(const std::string& title, Container* parent)
	: StyledContainer(parent)
    , m_titleAlign(TitleAlign::Left)
    , m_horizontalPadding(0)
    , m_verticalPadding(0)
	, m_title(title)
{
}

void ck::Panel::setTitle(const std::string& title)
{
	m_title = title;
}

void ck::Panel::setHeader(const std::string& text)
{
    m_headerLines = detail::splitLines(text);
}

void ck::Panel::setFooter(const std::string& text)
{
    m_footerLines = detail::splitLines(text);
}

void ck::Panel::setTitleAlign(TitleAlign align)
{
    m_titleAlign = align;
}

void ck::Panel::setPadding(int horizontal, int vertical)
{
    if (horizontal < 0) throw std::invalid_argument("Invalid horizontal padding value");
    if (vertical < 0) throw std::invalid_argument("Invalid vertical padding value");
    m_horizontalPadding = horizontal;
    m_verticalPadding = vertical;   
}

void ck::Panel::clearContent()
{
    m_headerLines.clear();
    m_footerLines.clear();
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

    int contentWidth = titleVisLen;

    std::vector<std::string> lines;
    for (const auto& l : m_headerLines) {
        contentWidth = std::max(detail::visible_length(l), contentWidth);
        lines.push_back(l);
    }
    if (!m_components.empty()) {
        for (auto& l : detail::splitLines(m_components[0]->draw(innerCtx))) {
            contentWidth = std::max(detail::visible_length(l), contentWidth);
            lines.push_back(std::move(l));
        }
    }
    for (const auto& l : m_footerLines) {
        contentWidth = std::max(detail::visible_length(l), contentWidth);
        lines.push_back(l);
    }

    if (lines.empty() && m_title.empty()) return "";

    int innerWidth = contentWidth + m_horizontalPadding * 2;

    std::string output = pColor;

    output += "+-";
    if (!m_title.empty()) {
        int fill = innerWidth - titleVisLen;
        if (m_titleAlign == TitleAlign::Left) {
            output += m_title + " " + std::string(std::max(0, fill - 1), '-');
        }
        else if (m_titleAlign == TitleAlign::Center) {
            int left = fill / 2;
            int right = fill - left;
            output += std::string(std::max(0, left - 1), '-') + " " + m_title + " " + std::string(std::max(0, right - 1), '-');
        }
        else {
            output += std::string(std::max(0, fill - 1), '-') + " " + m_title;
        }
    }
    else {
        output += std::string(innerWidth, '-');
    }
    output += "-+\n";

    for (int i = 0; i < m_verticalPadding; ++i)
        output += pColor + "| " + std::string(innerWidth, ' ') + " |\n";

    for (const auto& line : lines) {
        int visLen = detail::visible_length(line);
        int pad = contentWidth - visLen;
        output += pColor + "| ";
        output += std::string(m_horizontalPadding, ' ');
        output += detail::RESET;
        output += line;
        output += pColor;
        output += std::string(pad, ' ');
        output += std::string(m_horizontalPadding, ' ');
        output += " |\n";
    }

    for (int i = 0; i < m_verticalPadding; ++i)
        output += pColor + "| " + std::string(innerWidth, ' ') + " |\n";

    output += pColor + "+-" + std::string(innerWidth, '-') + "-+";
    output += ctx.apply();
    return output;
}

int ck::Panel::getHeight() const
{
    int contentHeight = m_headerLines.size() + m_footerLines.size();
    if (!m_components.empty()) {
        contentHeight += m_components[0]->getHeight();
    }
    return contentHeight + 2 * m_verticalPadding + 2;
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
