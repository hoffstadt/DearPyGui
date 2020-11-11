#include "mvColumns.h"

namespace Marvel {

	mvManagedColumns::mvManagedColumns(const std::string& name, int columns)
		: mvAppItem(name)
	{
		if (columns < 1)
			m_columns = 1;
		else if (columns > 64)
			m_columns = 64;
		else
			m_columns = columns;

		for (int i = 0; i < m_columns; i++)
			m_widths.push_back(0);
		m_description.container = true;
	}

	void mvManagedColumns::setColumnWidth(int i, float width)
	{
		if (i < 0 || i >= (int)m_widths.size())
			return;

		m_dirty_widths = true;

		m_widths[i] = width;
	}

	float mvManagedColumns::getColumnWidth(int i)
	{
		if (i < 0 || i >= (int)m_widths.size())
			return 0.0f;

		return m_widths[i];
	}

	void mvManagedColumns::draw()
	{
		m_previousColCount = ImGui::GetColumnsCount();
		ScopedID id;

		ImGui::Columns(m_columns, m_name.c_str(), m_border);
		for (mvAppItem* item : m_children)
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			// set item width
			if (item->getWidth() != 0)
				ImGui::SetNextItemWidth((float)item->getWidth());

			//item->pushColorStyles();
			item->draw();
			//item->popColorStyles();

			item->getState().update();

			int index = ImGui::GetColumnIndex();

			if (m_dirty_widths && !m_firstFrame)
				ImGui::SetColumnWidth(index, m_widths[index]);

			if (!m_firstFrame)
				m_widths[index] = ImGui::GetColumnWidth();
			ImGui::NextColumn();
		}

		// this is odd but is necessary for column widths to 
		// work properly
		if (m_firstFrame)
			m_firstFrame = false;
		else
			m_dirty_widths = false;

		ImGui::Columns(m_previousColCount);

	}

	void mvManagedColumns::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "border")) m_border = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "columns"))
		{
			m_columns = ToInt(item);

			if (m_columns < 1)
				m_columns = 1;
			else if (m_columns > 64)
				m_columns = 64;

			m_widths.clear();
			for (int i = 0; i < m_columns; i++)
				m_widths.push_back(0);
		}
	}

	void mvManagedColumns::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		PyDict_SetItemString(dict, "columns", ToPyInt(m_columns));
	}

	mvColumn::mvColumn(const std::string& name, int columns)
		: mvAppItem(name)
	{
		if (columns < 1)
			m_columns = 1;
		else if (columns > 64)
			m_columns = 64;
		else
			m_columns = columns;
	}

	void mvColumn::draw()
	{
		ScopedID id;
		ImGui::Columns(m_columns, m_name.c_str(), m_border);
	}

	void mvColumn::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "border")) m_border = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "columns"))
		{
			m_columns = ToInt(item);

			if (m_columns < 1)
				m_columns = 1;
			else if (m_columns > 64)
				m_columns = 64;
		}
	}

	void mvColumn::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		PyDict_SetItemString(dict, "columns", ToPyInt(m_columns));
	}

	mvNextColumn::mvNextColumn(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvNextColumn::draw()
	{
		ImGui::NextColumn();
	}
}