#include "mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel{

	mvAppItem::mvAppItem(const std::string& parent, const std::string& name)
		: m_name(name), m_label(name), m_tip(""), m_show(true),
		m_parent(mvApp::GetApp()->getItem(parent))
	{

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
		if(m_colorStyles.size() > 0)
			ImGui::PopStyleColor(m_colorStyles.size());
	}

	mvAppItem* mvAppItem::getChild(const std::string& name)
	{
		for (mvAppItem* item : m_children)
		{
			if (item->getName() == name)
				return item;

			if (item->isContainer())
			{
				auto child = item->getChild(name);
				if (child)
					return child;
			}
		}

		return nullptr;
	}

	void mvAppItem::addChild(mvAppItem* child)
	{
		m_container = true;
		m_children.push_back(child);
	}

}