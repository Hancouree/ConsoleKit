#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../core/Component.h"
#include "../core/Common.h"

namespace ck {
	class TreeNode {
	public:
		TreeNode(const std::string& label, std::function<void()> onDirty = nullptr);

		TreeNode* addChild(const std::string& label);
		const std::string& getLabel() const { return m_label; }
		const std::vector<std::unique_ptr<TreeNode>>& getChildren() const { return m_children; }
	private:
		void markDirty();

		std::string m_label;
		std::vector<std::unique_ptr<TreeNode>> m_children;
		std::function<void()> m_onDirty;
	};

	class Tree final : public StyledComponent
	{
	public:
		Tree(const std::string& label = "", Container* parent = nullptr);

		void setTheme(Theme theme) override;

		std::string draw(const StyleContext& ctx = {}) const override;
		TreeNode* getRoot() { return &m_root; };
		int getHeight() const override;
	private:
		void drawNode(const TreeNode& node, const std::string& prefix, bool isLast, std::string& output) const;
		int countNodes(const TreeNode& node) const;

		TreeNode m_root;

		static constexpr char ASCII_PIPE = '|';
		static constexpr char ASCII_BRANCH = '+';
		static constexpr char ASCII_LAST = '\\';
		static constexpr char SPACE = ' ';

		static constexpr const char* UNICODE_PIPE = "│"; 
		static constexpr const char* UNICODE_BRANCH = "├"; 
		static constexpr const char* UNICODE_LAST = "└"; 
		static constexpr const char* UNICODE_DASH = "─";
	};
}
