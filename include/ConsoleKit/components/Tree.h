#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../core/Component.h"

namespace ck {
	class TreeNode {
	public:
		TreeNode(const std::string& label);

		TreeNode* addChild(const std::string& label);
		const std::string& getLabel() const { return m_label; }
		const std::vector<std::unique_ptr<TreeNode>>& getChildren() const { return m_children; }
	private:
		std::string m_label;
		std::vector<std::unique_ptr<TreeNode>> m_children;
	};

	class Tree final : public StyledComponent
	{
	public:
		Tree(const std::string& label = "", Container* parent = nullptr);

		void setGuideChar(char pipe, char branch, char last, char space);

		std::string draw(const StyleContext& ctx = {}) const override;
		TreeNode* getRoot() { return &m_root; };
		int getHeight() const override;
	private:
		void drawNode(const TreeNode& node, const std::string& prefix, bool isLast, std::string& output) const;
		int countNodes(const TreeNode& node) const;

		TreeNode m_root;
		char m_pipe = '|';
		char m_branch = '+';
		char m_last = '\\';
		char m_space = ' ';
	};
}
