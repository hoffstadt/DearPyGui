#include "mvTableColumn.h"
#include "mvTable.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	mvTableColumn::mvTableColumn(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvTableColumn::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvTableColumn*>(item);
		_flags = titem->_flags;
		_init_width_or_weight = titem->_init_width_or_weight;
	}

	void mvTableColumn::draw(ImDrawList* drawlist, float x, float y)
	{
		_id = (ImGuiID)uuid;
		ImGui::TableSetupColumn(info.internalLabel.c_str(), _flags, _init_width_or_weight, _id);

		if (info.shownLastFrame)
		{
			info.shownLastFrame = false;
			ImGui::TableSetColumnEnabled(info.location, config.enabled);
		}

		if (info.hiddenLastFrame)
		{
			info.hiddenLastFrame = false;
			ImGui::TableSetColumnEnabled(info.location, config.enabled);
		}
	}

	void mvTableColumn::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "init_width_or_weight")) _init_width_or_weight = ToFloat(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("default_hide", ImGuiTableColumnFlags_DefaultHide, _flags);
		flagop("default_sort", ImGuiTableColumnFlags_DefaultSort, _flags);
		flagop("width_stretch", ImGuiTableColumnFlags_WidthStretch, _flags);
		flagop("width_fixed", ImGuiTableColumnFlags_WidthFixed, _flags);
		flagop("no_resize", ImGuiTableColumnFlags_NoResize, _flags);
		flagop("no_reorder", ImGuiTableColumnFlags_NoReorder, _flags);
		flagop("no_hide", ImGuiTableColumnFlags_NoHide, _flags);
		flagop("no_clip", ImGuiTableColumnFlags_NoClip, _flags);
		flagop("no_sort", ImGuiTableColumnFlags_NoSort, _flags);
		flagop("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, _flags);
		flagop("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, _flags);
		flagop("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, _flags);
		flagop("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, _flags);
		flagop("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, _flags);
		flagop("indent_enable", ImGuiTableColumnFlags_IndentEnable, _flags);
		flagop("indent_disable", ImGuiTableColumnFlags_IndentDisable, _flags);

	}

	void mvTableColumn::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_init_width_or_weight = ToPyFloat(_init_width_or_weight);

		PyDict_SetItemString(dict, "init_width_or_weight", py_init_width_or_weight);

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			mvPyObject py_value = ToPyBool(flags & flag);
			PyDict_SetItemString(dict, keyword, py_value);
		};

		checkbitset("default_hide", ImGuiTableColumnFlags_DefaultHide, _flags);
		checkbitset("default_sort", ImGuiTableColumnFlags_DefaultSort, _flags);
		checkbitset("width_stretch", ImGuiTableColumnFlags_WidthStretch, _flags);
		checkbitset("width_fixed", ImGuiTableColumnFlags_WidthFixed, _flags);
		checkbitset("no_resize", ImGuiTableColumnFlags_NoResize, _flags);
		checkbitset("no_reorder", ImGuiTableColumnFlags_NoReorder, _flags);
		checkbitset("no_hide", ImGuiTableColumnFlags_NoHide, _flags);
		checkbitset("no_clip", ImGuiTableColumnFlags_NoClip, _flags);
		checkbitset("no_sort", ImGuiTableColumnFlags_NoSort, _flags);
		checkbitset("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, _flags);
		checkbitset("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, _flags);
		checkbitset("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, _flags);
		checkbitset("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, _flags);
		checkbitset("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, _flags);
		checkbitset("indent_enable", ImGuiTableColumnFlags_IndentEnable, _flags);
		checkbitset("indent_disable", ImGuiTableColumnFlags_IndentDisable, _flags);
	}

}