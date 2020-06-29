#include "mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel{

	mvAppItem::mvAppItem(const std::string& parent, const std::string& name)
	{
		m_name = name;
		m_label = name;
	}

	bool mvAppItem::deleteChild(const std::string& name)
	{

		bool childfound = false;
		bool itemDeleted = false;

		for (mvAppItem* item : m_children)
		{
			if (item->getName() == name)
			{
				childfound = true;
				break;
			}

			if (item->isContainer())
			{
				itemDeleted = item->deleteChild(name);
				if (itemDeleted)
					break;
			}
		}

		if (childfound)
		{
			std::vector<mvAppItem*> oldchildren = m_children;

			m_children.clear();

			for (auto item : oldchildren)
			{
				if (item->getName() == name)
				{
					delete item;
					item = nullptr;
					itemDeleted = true;
					continue;
				}

				m_children.push_back(item);
			}
		}

		return itemDeleted;
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

	mvAppItem::~mvAppItem()
	{
		for (auto child : m_children)
		{
			delete child;
			child = nullptr;
		}

		m_children.clear();
	}

}