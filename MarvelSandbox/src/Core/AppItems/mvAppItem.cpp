#include "mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel{

	mvAppItem::mvAppItem(const std::string& parent, const std::string& name)
		: m_name(name), m_label(name), m_tip(""), m_show(true),
		m_parent(mvApp::GetApp()->getItem(parent))
	{

		if (m_parent)
			m_parent->m_children.push_back(this);
	}

	void mvAppItem::showAll()
	{
		m_show = true;
		for (auto child : m_children)
			child->showAll();
	}

	void mvAppItem::hideAll()
	{
		if (getType() != mvAppItemType::Tooltip)
			m_show = false;

		for (auto child : m_children)
			child->hideAll();
	}

	void mvAppItem::setParent(mvAppItem* parent)
	{
		if (parent)
			parent->m_children.push_back(this);

		m_parent = parent;
	}

	void mvAppItem::addColorStyle(ImGuiCol item, mvColor color)
	{
		m_colorStyles[item] = color;
	}

	void mvAppItem::pushColorStyles()
	{
		for (auto& item : m_colorStyles)
			ImGui::PushStyleColor(item.first, item.second);
	}

	void mvAppItem::popColorStyles()
	{
		ImGui::PopStyleColor(m_colorStyles.size());
	}

}