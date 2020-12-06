#include "mvContainerItems.h"
#include "core/mvInput.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	mvChild::mvChild(const std::string& name)
		: mvBoolPtrBase(name, false, name), mvOldEventHandler()
	{
		m_description.container = true;
	}

	void mvChild::addFlag(ImGuiWindowFlags flag) 
	{ 
		m_windowflags |= flag; 
	}

	void mvChild::removeFlag(ImGuiWindowFlags flag) 
	{ 
		m_windowflags &= ~flag; 
	}

	void mvChild::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		ImGui::BeginChild(m_label.c_str(), ImVec2(m_autosize_x ? 0 : (float)m_width, m_autosize_y ? 0 : (float)m_height), m_border, m_windowflags);

		for (mvAppItem* item : m_children)
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// set item width
			if (item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->draw();

			// Regular Tooltip (simple)
			if (!item->getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", item->getTip().c_str());

			item->getState().update();
		}

		// TODO check if these work for child
		if (!m_tip.empty() && ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", m_tip.c_str());

		// allows this item to have a render callback
		registerWindowFocusing();

		m_width = (int)ImGui::GetWindowWidth();
		m_height = (int)ImGui::GetWindowHeight();

		ImGui::EndChild();
	}

	void mvChild::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "border")) m_border = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_x")) m_autosize_x = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize_y")) m_autosize_y = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
	}

	void mvChild::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		PyDict_SetItemString(dict, "autosize_x", ToPyBool(m_autosize_x));
		PyDict_SetItemString(dict, "autosize_y", ToPyBool(m_autosize_y));

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
	}

	mvGroup::mvGroup(const std::string& name)
		: mvAppItem(name)
	{
		m_description.container = true;
	}

	void mvGroup::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();

		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);

		ImGui::BeginGroup();

		for (mvAppItem* item : m_children)
		{
			if (m_width != 0)
				item->setWidthND(m_width);

			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// set item width
			if (item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			item->draw();

			// Regular Tooltip (simple)
			if (!item->getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", item->getTip().c_str());

			if (m_horizontal)
				ImGui::SameLine(0.0, m_hspacing);

			item->getState().update();
		}

		if (m_width != 0)
			ImGui::PopItemWidth();

		ImGui::EndGroup();

		if (!m_tip.empty() && ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", m_tip.c_str());

	}

	void mvGroup::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) m_hspacing = ToFloat(item);
	}

	void mvGroup::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
		PyDict_SetItemString(dict, "horizontal_spacing", ToPyFloat(m_hspacing));
	}

	mvCollapsingHeader::mvCollapsingHeader(const std::string& name)
		: mvBoolPtrBase(name, true, name)
	{
		m_description.container = true;
	}

	void mvCollapsingHeader::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		bool* toggle = nullptr;
		if (m_closable)
			toggle = m_value;

		if (ImGui::CollapsingHeader(m_label.c_str(), toggle, m_flags))
		{

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();
			}
		}

		else
		{
			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	}

	void mvCollapsingHeader::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);


	}

	void mvCollapsingHeader::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);

	}

	mvTreeNode::mvTreeNode(const std::string& name)
		: mvBoolPtrBase(name, false, name)
	{
		m_description.container = true;
	}

	void mvTreeNode::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		ImGui::BeginGroup();
		if (ImGui::TreeNodeEx(m_label.c_str(), m_flags))
		{

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();
			}
			ImGui::TreePop();
		}

		// Regular Tooltip (simple)
		if (!getTip().empty() && ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", getTip().c_str());

		ImGui::EndGroup();
	}

	void mvTreeNode::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);
	}

	void mvTreeNode::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);

	}
}