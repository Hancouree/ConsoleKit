#include "../../include/ConsoleKit/layouts/HLayout.h"
#include <algorithm>

ck::HLayout::HLayout(Container* parent) : Layout(parent)
{
}

std::string ck::HLayout::draw(const StyleContext& ctx) const
{
	std::string output;

	std::vector<std::vector<std::string>> lines;
	std::vector<int> columnWidths;
	int maxHeight = 0;
	for (const auto& e : m_components) {
		maxHeight = std::max(maxHeight, e.c->getHeight());

		std::vector<std::string> l = detail::splitLines(e.c->draw(ctx));
		int columnWidth = 0;
		for (const auto& s : l) {
			columnWidth = std::max(columnWidth, detail::visible_length(s));
		}

		lines.push_back(std::move(l));
		columnWidths.push_back(columnWidth);
	}

	for (int currentHeight = 0; currentHeight < maxHeight; ++currentHeight) {
		for (size_t idx = 0; idx < lines.size(); ++idx) {
			int targetWidth = columnWidths[idx];

			if (lines[idx].size() > currentHeight) {
				output += lines[idx][currentHeight];

				int actualWidth = detail::visible_length(lines[idx][currentHeight]);
				if (actualWidth < targetWidth) {
					output += std::string(targetWidth - actualWidth, ' ');
				}
			}
			else {
				output += std::string(targetWidth, ' ');
			}

			if (idx < lines.size() - 1) {
				output += std::string(m_spacing, ' ');
			}
		}

		if (currentHeight < maxHeight - 1) {
			output += '\n';
		}
	}

	return output;
}

int ck::HLayout::getHeight() const
{
	if (m_components.empty()) return 0;

	auto it = std::max_element(m_components.begin(), m_components.end(),
		[](ComponentEntry a, ComponentEntry b) {
			return a.c->getHeight() < b.c->getHeight();
		});

	return it->c->getHeight();
}
