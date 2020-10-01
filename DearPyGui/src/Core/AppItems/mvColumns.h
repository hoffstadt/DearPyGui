#pragma once

#include "Core/AppItems/mvTypeBases.h"
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
			m_container = true;
		}

		void draw() override
		{
			m_previousColCount = ImGui::GetColumnsCount();
			ImGui::PushID(this);
			ImGui::Columns(m_columns, m_name.c_str(), m_border);
			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->pushColorStyles();
				item->draw();
				item->popColorStyles();
				

				item->setHovered(ImGui::IsItemHovered());
				item->setActive(ImGui::IsItemActive());
				item->setFocused(ImGui::IsItemFocused());
				item->setClicked(ImGui::IsItemClicked());
				item->setVisible(ImGui::IsItemVisible());
				item->setEdited(ImGui::IsItemEdited());
				item->setActivated(ImGui::IsItemActivated());
				item->setDeactivated(ImGui::IsItemDeactivated());
				item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
				item->setToggledOpen(ImGui::IsItemToggledOpen());
				item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
				item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
				item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });

				int indexxxx = ImGui::GetColumnIndex();
				float wide = ImGui::GetColumnWidth();
				m_widths[indexxxx] = wide;
				ImGui::NextColumn();
			}

			ImGui::PopID();

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
			ImGui::PushID(this);
			ImGui::Columns(m_columns, m_name.c_str(), m_border);
			ImGui::PopID();
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
