#include "../../include/ConsoleKit/components/Tree.h"
#include "../../include/ConsoleKit/core/Common.h"

ck::TreeNode::TreeNode(const std::string& label)
	: m_label(label)
{
}

ck::TreeNode* ck::TreeNode::addChild(const std::string& label)
{
	m_children.push_back(std::make_unique<TreeNode>(label));
	return m_children.back().get();
}

ck::Tree::Tree(const std::string& label, Container* parent)
	: StyledComponent(parent)
	, m_root(label)
{
}

void ck::Tree::setGuideChar(char pipe, char branch, char last, char space)
{
	m_pipe = pipe;
	m_branch = branch;
	m_last = last;
	m_space = space;
}

std::string ck::Tree::draw(const StyleContext& ctx) const
{
	std::string color = detail::color_to_ansi(m_color);
	std::string output = color + m_root.getLabel();

	for (size_t i = 0; i < m_root.getChildren().size(); ++i) {
		output += "\n";
		drawNode(*m_root.getChildren()[i], "", i == m_root.getChildren().size() - 1, output);
	}

	return output + ctx.apply();
}

int ck::Tree::getHeight() const
{
	return countNodes(m_root);
}

void ck::Tree::drawNode(const TreeNode& node, const std::string& prefix, bool isLast, std::string& output) const
{
	std::string color = detail::color_to_ansi(m_color);
	std::string connector = isLast
		? std::string(1, m_last) + "-- "
		: std::string(1, m_branch) + "-- ";

	output += color + prefix + connector + node.getLabel();

	std::string childPrefix = prefix + (isLast
		? std::string(1, m_space) + "   "
		: std::string(1, m_pipe) + "   ");

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
