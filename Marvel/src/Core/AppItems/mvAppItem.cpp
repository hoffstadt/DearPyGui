#include "mvAppItem.h"
#include "Core/mvApp.h"

namespace Marvel{

	mvAppItem::mvAppItem(const std::string& parent, const std::string& name)
	{
		m_name = name;
		m_label = name;
	}

	bool  mvAppItem::moveChildUp(const std::string& name)
	{
		bool found = false;
		int index = 0;

		// check children
		for (int i = 0; i<m_children.size(); i++)
		{

			if (m_children[i]->getName() == name)
			{
				found = true;
				index = i;
				break;
			}

			if (m_children[i]->isContainer())
			{
				found = m_children[i]->moveChildUp(name);
				if (found)
					return true;
			}

		}

		if (found)
		{
			if (index > 0)
			{
				auto upperitem = m_children[index - 1];
				auto loweritem = m_children[index];

				m_children[index] = upperitem;
				m_children[index-1] = loweritem;
			}

			return true;
		}

		return false;

	}

	bool  mvAppItem::moveChildDown(const std::string& name)
	{
		bool found = false;
		int index = 0;

		// check children
		for (int i = 0; i < m_children.size(); i++)
		{

			if (m_children[i]->getName() == name)
			{
				found = true;
				index = i;
				break;
			}

			if (m_children[i]->isContainer())
			{
				found = m_children[i]->moveChildDown(name);
				if (found)
					return true;
			}

		}

		if (found)
		{
			if (index < m_children.size()-1)
			{
				auto upperitem = m_children[index];
				auto loweritem = m_children[index+1];

				m_children[index] = loweritem;
				m_children[index + 1] = upperitem;
			}

			return true;
		}

		return false;
	}

	bool mvAppItem::addRuntimeChild(const std::string& parent, const std::string& after, mvAppItem* item)
	{
		if (after.empty() && parent.empty())
			return false;

		//this is the container, add item to beginning.
		if (after == "")
		{
			if (parent == m_name)
			{
				std::vector<mvAppItem*> oldchildren = m_children;
				m_children.clear();
				m_children.push_back(item);

				for (auto child : oldchildren)
					m_children.push_back(child);

				item->setParent(this);

				return true;
			}

			else
			{
				// check children
				for (mvAppItem* child : m_children)
				{

					bool parentFound = false;
					if (child->isContainer())
					{
						parentFound = child->addRuntimeChild(parent, after, item);

						if (parentFound)
							return true;
					}
				}
			}
		}

		else
		{
			bool afterFound = false;

			// check children
			for (mvAppItem* child : m_children)
			{

				if (child->getName() == after)
				{
					afterFound = true;
					break;
				}

			}

			// after item is in this container
			if (afterFound)
			{
				item->setParent(this);

				std::vector<mvAppItem*> oldchildren = m_children;
				m_children.clear();

				for (auto child : oldchildren)
				{
					m_children.push_back(child);
					if (child->getName() == after)
						m_children.push_back(item);

				}

				return true;
			}
		}

		// check children
		for (mvAppItem* child : m_children)
		{

			bool parentFound = false;
			if (child->isContainer())
			{
				parentFound = child->addRuntimeChild(parent, after, item);

				if (parentFound)
					return true;
			}
		}

		return false;
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