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
		if (getType() != mvAppItemType::Tooltip && getType() != mvAppItemType::EndTooltip)
			m_show = false;

		for (auto child : m_children)
			child->hideAll();
	}

}