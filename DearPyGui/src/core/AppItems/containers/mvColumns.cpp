#include "mvColumns.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvThemeManager.h"

namespace Marvel {

	void mvManagedColumns::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_managed_columns", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "columns"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "border", "show border", "True"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds managed columns.", "None", "Containers") });

		parsers->insert({ "get_managed_column_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "column"},
		}, "Returns the width of the ith column.", "Float", "Widget Commands") });

		parsers->insert({ "set_managed_column_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::Float, "width"},
		}, "Sets the width of the ith column.", "None", "Widget Commands") });
	}

	void mvColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_columns", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "columns"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "border", "show border", "True"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Sets columns.", "None", "Containers") });
	}

	void mvNextColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_next_column", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'next_collumn'"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Changes to next column.", "None", "Containers") });
	}

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
		mvImGuiThemeScope scope(this);

		ImGui::Columns(m_columns, m_core_config.name.c_str(), m_border);
		for (auto& item : m_children)
		{
			// skip item if it's not shown
			if (!item->m_core_config.show)
				continue;

			// set item width
			if (item->m_core_config.width != 0)
				ImGui::SetNextItemWidth((float)item->m_core_config.width);

			item->draw();

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
		ImGui::Columns(m_columns, m_core_config.name.c_str(), m_border);
	}

	mvNextColumn::mvNextColumn(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvNextColumn::draw()
	{
		ImGui::NextColumn();
	}

#ifndef MV_CPP

	PyObject* add_managed_columns(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int columns;
		int border = true;
		int show = true;
		const char* parent = "";
		const char* before = "";


		if (!(mvApp::GetApp()->getParsers())["add_managed_columns"].parse(args, kwargs, __FUNCTION__,
			&name, &columns, &border, &show, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvManagedColumns>(name, columns);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();
	}

	PyObject* add_next_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("next_column" + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())["add_next_column"].parse(args, kwargs, __FUNCTION__,
			&name, &show, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvNextColumn>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

	PyObject* add_columns(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int columns;
		int border = true;
		int show = true;
		const char* parent = "";
		const char* before = "";


		if (!(mvApp::GetApp()->getParsers())["add_columns"].parse(args, kwargs, __FUNCTION__,
			&name, &columns, &border, &show, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvColumn>(name, columns);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

	PyObject* get_managed_column_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* managed_columns;
		int column;

		if (!(mvApp::GetApp()->getParsers())["get_managed_column_width"].parse(args, kwargs, __FUNCTION__,
			&managed_columns, &column))
			return ToPyFloat(0.0f);


		std::string returnMessage;

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(managed_columns);

		if (appitem == nullptr)
		{
			ThrowPythonException(managed_columns + std::string(" managed_columns does not exist."));
			return ToPyFloat(0.0f);
		}

		mvManagedColumns* columns;
		if (appitem->getType() == mvAppItemType::ManagedColumns)
			columns = static_cast<mvManagedColumns*>(appitem.get());
		else
		{
			ThrowPythonException(std::string(managed_columns) + " is not a managed columns.");
			return ToPyFloat(0.0f);
		}

		return ToPyFloat(columns->getColumnWidth(column));
	}

	PyObject* set_managed_column_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* managed_columns;
		int column;
		float width;

		if (!(mvApp::GetApp()->getParsers())["set_managed_column_width"].parse(args, kwargs, __FUNCTION__,
			&managed_columns, &column, &width))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(managed_columns);

		if (appitem == nullptr)
		{
			std::string message = managed_columns;
			ThrowPythonException(message + " managed_columns does not exist.");
			return GetPyNone();
		}

		mvManagedColumns* columns;
		if (appitem->getType() == mvAppItemType::ManagedColumns)
		{
			columns = static_cast<mvManagedColumns*>(appitem.get());
			columns->setColumnWidth(column, width);
		}
		else
		{
			ThrowPythonException(std::string(managed_columns) + " is not a managed columns.");
			return GetPyNone();
		}

		return GetPyNone();
	}

	void mvManagedColumns::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

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

		PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		PyDict_SetItemString(dict, "columns", ToPyInt(m_columns));
	}

	void mvColumn::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

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

		PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		PyDict_SetItemString(dict, "columns", ToPyInt(m_columns));
	}

#endif // !MV_CPP
}