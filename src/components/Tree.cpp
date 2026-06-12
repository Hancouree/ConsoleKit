#include "../../include/ConsoleKit/components/Tree.h"
#include "../../include/ConsoleKit/core/Common.h"

ck::TreeNode::TreeNode(const std::string& label, std::function<void()> onDirty)
	: m_label(label)
	, m_onDirty(onDirty)
{
}

ck::TreeNode* ck::TreeNode::addChild(const std::string& label)
{
	m_children.push_back(std::make_unique<TreeNode>(label, m_onDirty));
	markDirty();
	return m_children.back().get();
}

void ck::TreeNode::markDirty()
{
	if (m_onDirty) m_onDirty();
}

ck::Tree::Tree(const std::string& label, Container* parent)
	: StyledComponent(parent)
	, m_root(label, [this]() { m_isDirty = true; })
{
}

void ck::Tree::setTheme(Theme theme)
{
	m_theme = theme;
	m_isDirty = true;
}

std::string ck::Tree::draw(const StyleContext& ctx) const
{
	if (m_isDirty) {
		std::string color = detail::color_to_ansi(m_color);
		std::string output = color + m_root.getLabel();

		for (size_t i = 0; i < m_root.getChildren().size(); ++i) {
			output += "\n";
			drawNode(*m_root.getChildren()[i], "", i == m_root.getChildren().size() - 1, output);
		}

		m_cachedOutput = output;
		m_isDirty = false;
	}

	return m_cachedOutput + ctx.apply();
}

int ck::Tree::getHeight() const
{
	return countNodes(m_root);
}

void ck::Tree::drawNode(const TreeNode& node, const std::string& prefix, bool isLast, std::string& output) const
{
	std::string color = detail::color_to_ansi(m_color);

	bool isAscii = m_theme == Theme::Ascii;

	std::string L = isAscii ? std::string{ ASCII_LAST } : UNICODE_LAST;
	std::string B = isAscii ? std::string{ ASCII_BRANCH } : UNICODE_BRANCH;
	std::string D = isAscii ? "--" : UNICODE_DASH;
	std::string P = isAscii ? std::string{ ASCII_PIPE } : UNICODE_PIPE;

	std::string connector = isLast
		? L + D + " "
		: B + D + " ";

	output += color + prefix + connector + node.getLabel();

	std::string childPrefix = prefix + (isLast
		? "   "
		: P + "   ");

	for (size_t i = 0; i < node.getChildren().size(); ++i) {
		output += "\n";
		drawNode(*node.getChildren()[i], childPrefix, i == node.getChildren().size() - 1, output);
	}
}

int ck::Tree::countNodes(const TreeNode& node) const
{
	int count = 1;
	for (const auto& child : node.getChildren()) {
		count += countNodes(*child);
	}

	return count;
}
