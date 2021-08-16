#include "mvTable.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvTableColumn.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvTable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Tables", "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("header_row", mvArgType::KEYWORD_ARG, "True", "show headers at the top of the columns");

		parser.addArg<mvPyDataType::Integer>("inner_width", mvArgType::KEYWORD_ARG, "0");
		parser.addArg<mvPyDataType::Integer>("policy", mvArgType::KEYWORD_ARG, "0");
		parser.addArg<mvPyDataType::Integer>("freeze_rows", mvArgType::KEYWORD_ARG, "0");
		parser.addArg<mvPyDataType::Integer>("freeze_columns", mvArgType::KEYWORD_ARG, "0");

		parser.addArg<mvPyDataType::Bool>("sort_multi", mvArgType::KEYWORD_ARG, "False", "Hold shift when clicking headers to sort on multiple column.");
		parser.addArg<mvPyDataType::Bool>("sort_tristate", mvArgType::KEYWORD_ARG, "False", "Allow no sorting, disable default sorting.");

		parser.addArg<mvPyDataType::Bool>("resizable", mvArgType::KEYWORD_ARG, "False", "Enable resizing columns");
		parser.addArg<mvPyDataType::Bool>("reorderable", mvArgType::KEYWORD_ARG, "False", "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)");
		parser.addArg<mvPyDataType::Bool>("hideable", mvArgType::KEYWORD_ARG, "False", "Enable hiding/disabling columns in context menu.");
		parser.addArg<mvPyDataType::Bool>("sortable", mvArgType::KEYWORD_ARG, "False", "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.");
		parser.addArg<mvPyDataType::Bool>("context_menu_in_body", mvArgType::KEYWORD_ARG, "False", "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().");
		parser.addArg<mvPyDataType::Bool>("row_background", mvArgType::KEYWORD_ARG, "False", "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)");
		parser.addArg<mvPyDataType::Bool>("borders_innerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders between rows.");
		parser.addArg<mvPyDataType::Bool>("borders_outerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders at the top and bottom.");
		parser.addArg<mvPyDataType::Bool>("borders_innerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders between columns.");
		parser.addArg<mvPyDataType::Bool>("borders_outerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders on the left and right sides.");

		parser.addArg<mvPyDataType::Bool>("no_host_extendX", mvArgType::KEYWORD_ARG, "False", "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
		parser.addArg<mvPyDataType::Bool>("no_host_extendY", mvArgType::KEYWORD_ARG, "False", "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
		parser.addArg<mvPyDataType::Bool>("no_keep_columns_visible", mvArgType::KEYWORD_ARG, "False", "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.");
		parser.addArg<mvPyDataType::Bool>("precise_widths", mvArgType::KEYWORD_ARG, "False", "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
		parser.addArg<mvPyDataType::Bool>("no_clip", mvArgType::KEYWORD_ARG, "False", "Disable clipping rectangle for every individual columns.");
		parser.addArg<mvPyDataType::Bool>("pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.");
		parser.addArg<mvPyDataType::Bool>("no_pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is off. Disable outer-most padding.");
		parser.addArg<mvPyDataType::Bool>("no_pad_innerX", mvArgType::KEYWORD_ARG, "False", "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).");
		parser.addArg<mvPyDataType::Bool>("scrollX", mvArgType::KEYWORD_ARG, "False", "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX.");
		parser.addArg<mvPyDataType::Bool>("scrollY", mvArgType::KEYWORD_ARG, "False", "Enable vertical scrolling.");
		parser.addArg<mvPyDataType::Bool>("no_saved_settings", mvArgType::KEYWORD_ARG, "False", "Never load/save settings in .ini file.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTable::mvTable(mvUUID uuid)
		: mvAppItem(uuid)
	{
		//_label = "table" + std::to_string(uuid);
	}

	void mvTable::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (_columns == 0)
			return;

		if (ImGui::BeginTable(_internalLabel.c_str(), _columns, _flags, 
			ImVec2((float)_width, (float)_height), (float)_inner_width))
		{

			ImGui::TableSetupScrollFreeze(_freezeRows, _freezeColumns);

			// columns
			for (auto& item : _children[0])
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				//if (item->_width != 0)
				//	ImGui::SetNextItemWidth((float)item->_width);

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}

			if (_tableHeader)
				ImGui::TableHeadersRow();

			if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
			{
				if (sorts_specs->SpecsDirty)
				{
					if (sorts_specs->SpecsCount == 0)
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, nullptr, _user_data);
					else
					{

						// generate id map for columns
						std::unordered_map<ImGuiID, mvUUID> idMap;
						for (size_t i = 0; i < _children[0].size(); i++)
							idMap[static_cast<mvTableColumn*>(_children[0][i].get())->_id] = _children[0][i]->getUUID();

						std::vector<SortSpec> specs;
						for (int i = 0; i < sorts_specs->SpecsCount; i++)
						{
							const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[i];
							specs.push_back({ idMap[sort_spec->ColumnUserID], sort_spec->SortDirection == ImGuiSortDirection_Ascending ? 1 : -1 });
						}

						mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
							PyObject* pySpec = PyList_New(specs.size());
							for (size_t i = 0; i < specs.size(); i++)
							{
								PyObject* pySingleSpec = PyList_New(2);
								PyList_SetItem(pySingleSpec, 0, ToPyUUID(specs[i].column));
								PyList_SetItem(pySingleSpec, 1, ToPyInt(specs[i].direction));
								PyList_SetItem(pySpec, i, pySingleSpec);
							}
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, pySpec, _user_data);
							Py_XDECREF(pySpec);
							});
					}
					sorts_specs->SpecsDirty = false;
				}
			}

			if (!_children[1].empty())
			{
				// TODO: hacky solution, needs work
				if(_children[1][0]->getType() != mvAppItemType::mvTableRow && 
					_children[1][0]->getType() != mvAppItemType::mvClipper &&
					_children[1][0]->getType() != mvAppItemType::mvFilterSet)
					ImGui::TableNextColumn();
			}
			

			for (auto& item : _children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}
			ImGui::EndTable();
		}

	}

	void mvTable::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvTableColumn)
			_columns++;
	}

	void mvTable::onChildRemoved(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvTableColumn)
			_columns--;
	}

	void mvTable::onChildrenRemoved()
	{
		_columns = _children[0].size();
	}

	void mvTable::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "freeze_rows")) _freezeRows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "freeze_columns")) _freezeColumns = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "header_row")) _tableHeader = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "inner_width")) _inner_width = (int)ToFloat(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("resizable", ImGuiTableFlags_Resizable, _flags);
		flagop("reorderable", ImGuiTableFlags_Reorderable, _flags);
		flagop("hideable", ImGuiTableFlags_Hideable, _flags);
		flagop("sortable", ImGuiTableFlags_Sortable, _flags);
		flagop("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, _flags);
		flagop("row_background", ImGuiTableFlags_RowBg, _flags);
		flagop("borders_innerH", ImGuiTableFlags_BordersInnerH, _flags);
		flagop("borders_outerH", ImGuiTableFlags_BordersOuterH, _flags);
		flagop("borders_innerV", ImGuiTableFlags_BordersInnerV, _flags);
		flagop("borders_outerV", ImGuiTableFlags_BordersOuterV, _flags);
		flagop("no_host_extendX", ImGuiTableFlags_NoHostExtendX, _flags);
		flagop("no_host_extendY", ImGuiTableFlags_NoHostExtendY, _flags);
		flagop("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, _flags);
		flagop("precise_widths", ImGuiTableFlags_PreciseWidths, _flags);
		flagop("no_clip", ImGuiTableFlags_NoClip, _flags);
		flagop("pad_outerX", ImGuiTableFlags_PadOuterX, _flags);
		flagop("no_pad_outerX", ImGuiTableFlags_NoPadOuterX, _flags);
		flagop("no_pad_innerX", ImGuiTableFlags_NoPadInnerX, _flags);
		flagop("scrollX", ImGuiTableFlags_ScrollX, _flags);
		flagop("scrollY", ImGuiTableFlags_ScrollY, _flags);
		flagop("sort_multi", ImGuiTableFlags_SortMulti, _flags);
		flagop("sort_tristate", ImGuiTableFlags_SortTristate, _flags);
		flagop("no_saved_settings", ImGuiTableFlags_NoSavedSettings, _flags);

		if (PyObject* item = PyDict_GetItemString(dict, "policy"))
		{

			int policy = ToInt(item);

			// remove old flags
			_flags &= ~ImGuiTableFlags_SizingFixedFit;
			_flags &= ~ImGuiTableFlags_SizingFixedSame;
			_flags &= ~ImGuiTableFlags_SizingStretchProp;
			_flags &= ~ImGuiTableFlags_SizingStretchSame;

			if (policy == ImGuiTableFlags_SizingFixedFit)
				_flags |= ImGuiTableFlags_SizingFixedFit;
			else if(policy == ImGuiTableFlags_SizingFixedSame)
				_flags |= ImGuiTableFlags_SizingFixedSame;
			else if (policy == ImGuiTableFlags_SizingStretchProp)
				_flags |= ImGuiTableFlags_SizingStretchProp;
			else
				_flags |= ImGuiTableFlags_SizingStretchSame;

		}

		//if (_flags & ImGuiTableFlags_BordersInnerV)
		//	_flags &= ~ImGuiTableFlags_Resizable;
}

	void mvTable::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_freeze_rows = ToPyInt(_freezeRows);
		mvPyObject py_freeze_columns = ToPyInt(_freezeColumns);
		mvPyObject py_inner_width = ToPyInt(_inner_width);
		mvPyObject py_header_row = ToPyBool(_tableHeader);

		PyDict_SetItemString(dict, "freeze_rows", py_freeze_rows);
		PyDict_SetItemString(dict, "freeze_columns", py_freeze_columns);
		PyDict_SetItemString(dict, "inner_width", py_inner_width);
		PyDict_SetItemString(dict, "header_row", py_header_row);

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			mvPyObject py_value = ToPyBool(flags & flag);
			PyDict_SetItemString(dict, keyword, py_value);
		};

		checkbitset("resizable", ImGuiTableFlags_Resizable, _flags);
		checkbitset("reorderable", ImGuiTableFlags_Reorderable, _flags);
		checkbitset("hideable", ImGuiTableFlags_Hideable, _flags);
		checkbitset("sortable", ImGuiTableFlags_Sortable, _flags);
		checkbitset("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, _flags);
		checkbitset("row_background", ImGuiTableFlags_RowBg, _flags);
		checkbitset("borders_innerH", ImGuiTableFlags_BordersInnerH, _flags);
		checkbitset("borders_outerH", ImGuiTableFlags_BordersOuterH, _flags);
		checkbitset("borders_innerV", ImGuiTableFlags_BordersInnerV, _flags);
		checkbitset("borders_outerV", ImGuiTableFlags_BordersOuterV, _flags);
		checkbitset("no_host_extendX", ImGuiTableFlags_NoHostExtendX, _flags);
		checkbitset("no_host_extendY", ImGuiTableFlags_NoHostExtendY, _flags);
		checkbitset("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, _flags);
		checkbitset("precise_widths", ImGuiTableFlags_PreciseWidths, _flags);
		checkbitset("no_clip", ImGuiTableFlags_NoClip, _flags);
		checkbitset("pad_outerX", ImGuiTableFlags_PadOuterX, _flags);
		checkbitset("no_pad_outerX", ImGuiTableFlags_NoPadOuterX, _flags);
		checkbitset("no_pad_innerX", ImGuiTableFlags_NoPadInnerX, _flags);
		checkbitset("scrollX", ImGuiTableFlags_ScrollX, _flags);
		checkbitset("scrollY", ImGuiTableFlags_ScrollY, _flags);
		checkbitset("sort_multi", ImGuiTableFlags_SortMulti, _flags);
		checkbitset("sort_tristate", ImGuiTableFlags_SortTristate, _flags);
		checkbitset("no_saved_settings", ImGuiTableFlags_NoSavedSettings, _flags);
		
		int flag = 0;

		if(_flags & ImGuiTableFlags_SizingFixedFit)
			flag = ImGuiTableFlags_SizingFixedFit;
		else if (_flags & ImGuiTableFlags_SizingFixedSame)
			flag = ImGuiTableFlags_SizingFixedSame;
		else if (_flags & ImGuiTableFlags_SizingStretchProp)
			flag = ImGuiTableFlags_SizingStretchProp;
		else if (_flags & ImGuiTableFlags_SizingStretchSame)
			flag = ImGuiTableFlags_SizingStretchSame;

		mvPyObject py_policy = ToPyInt(flag);
		PyDict_SetItemString(dict, "policy", py_policy);
	}

}