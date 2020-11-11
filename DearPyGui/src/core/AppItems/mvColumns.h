#pragma once

#include "core/AppItems/mvTypeBases.h"
#include <vector>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvManagedColumns
	//-----------------------------------------------------------------------------
	class mvManagedColumns : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ManagedColumns, "add_managed_columns")

		mvManagedColumns(const std::string& name, int columns)
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

		void setColumnWidth(int i, float width)
		{
			if (i < 0 || i >= (int)m_widths.size())
				return;

			m_dirty_widths = true;

			m_widths[i] = width;
		}

		float getColumnWidth(int i)
		{
			if (i < 0 || i >= (int)m_widths.size())
				return 0.0f;

			return m_widths[i];
		}

		void draw() override
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

				if(!m_firstFrame)
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

		void setExtraConfigDict(PyObject* dict) override
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

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "border", ToPyBool(m_border));
			PyDict_SetItemString(dict, "columns", ToPyInt(m_columns));
		}

	private:

		int                m_previousColCount = 1;
		int                m_columns = 1;
		bool               m_border = true;
		std::vector<float> m_widths;
		bool               m_dirty_widths = false;
		bool               m_firstFrame = true; // only necessary for column widths

	};

	//-----------------------------------------------------------------------------
	// mvColumn
	//-----------------------------------------------------------------------------
	class mvColumn : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColumnSet, "add_columns")

			mvColumn(const std::string& name, int columns)
			: mvAppItem(name)
		{
			if (columns < 1)
				m_columns = 1;
			else if (columns > 64)
				m_columns = 64;
			else
				m_columns = columns;
		}

		void draw() override
		{
			ScopedID id;
			ImGui::Columns(m_columns, m_name.c_str(), m_border);
		}

		void setExtraConfigDict(PyObject* dict) override
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

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "border", ToPyBool(m_border));
			PyDict_SetItemString(dict, "columns", ToPyInt(m_columns));
		}

	private:

		int                m_columns = 1;
		bool               m_border = true;

	};

	//-----------------------------------------------------------------------------
	// mvNextColumn
	//-----------------------------------------------------------------------------
	class mvNextColumn : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::NextColumn, "add_next_column")

		mvNextColumn(const std::string& name)
			: mvAppItem(name)
		{
		}

		void draw() override
		{
			ImGui::NextColumn();
		}

	};

}
