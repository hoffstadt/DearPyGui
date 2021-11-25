#include "mvTable.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvTableColumn.h"
#include "mvPyObject.h"
#include "mvPythonExceptions.h"
#include "fonts/mvFont.h"
#include "themes/mvTheme.h"

namespace Marvel {

	void mvTable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			std::vector<mvPythonDataElement> args;
			AddCommonArgs(args,(CommonParserArgs)(
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

			args.push_back({ mvPyDataType::Bool, "header_row", mvArgType::KEYWORD_ARG, "True", "show headers at the top of the columns" });
			args.push_back({ mvPyDataType::Bool, "clipper", mvArgType::KEYWORD_ARG, "False", "Use clipper (rows must be same height)." });
			args.push_back({ mvPyDataType::Integer, "inner_width", mvArgType::KEYWORD_ARG, "0" });
			args.push_back({ mvPyDataType::Integer, "policy", mvArgType::KEYWORD_ARG, "0" });
			args.push_back({ mvPyDataType::Integer, "freeze_rows", mvArgType::KEYWORD_ARG, "0" });
			args.push_back({ mvPyDataType::Integer, "freeze_columns", mvArgType::KEYWORD_ARG, "0" });
			args.push_back({ mvPyDataType::Bool, "sort_multi", mvArgType::KEYWORD_ARG, "False", "Hold shift when clicking headers to sort on multiple column." });
			args.push_back({ mvPyDataType::Bool, "sort_tristate", mvArgType::KEYWORD_ARG, "False", "Allow no sorting, disable default sorting." });
			args.push_back({ mvPyDataType::Bool, "resizable", mvArgType::KEYWORD_ARG, "False", "Enable resizing columns" });
			args.push_back({ mvPyDataType::Bool, "reorderable", mvArgType::KEYWORD_ARG, "False", "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)" });
			args.push_back({ mvPyDataType::Bool, "hideable", mvArgType::KEYWORD_ARG, "False", "Enable hiding/disabling columns in context menu." });
			args.push_back({ mvPyDataType::Bool, "sortable", mvArgType::KEYWORD_ARG, "False", "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate." });
			args.push_back({ mvPyDataType::Bool, "context_menu_in_body", mvArgType::KEYWORD_ARG, "False", "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow()." });
			args.push_back({ mvPyDataType::Bool, "row_background", mvArgType::KEYWORD_ARG, "False", "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)" });
			args.push_back({ mvPyDataType::Bool, "borders_innerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders between rows." });
			args.push_back({ mvPyDataType::Bool, "borders_outerH", mvArgType::KEYWORD_ARG, "False", "Draw horizontal borders at the top and bottom." });
			args.push_back({ mvPyDataType::Bool, "borders_innerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders between columns." });
			args.push_back({ mvPyDataType::Bool, "borders_outerV", mvArgType::KEYWORD_ARG, "False", "Draw vertical borders on the left and right sides." });
			args.push_back({ mvPyDataType::Bool, "no_host_extendX", mvArgType::KEYWORD_ARG, "False", "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used." });
			args.push_back({ mvPyDataType::Bool, "no_host_extendY", mvArgType::KEYWORD_ARG, "False", "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible." });
			args.push_back({ mvPyDataType::Bool, "no_keep_columns_visible", mvArgType::KEYWORD_ARG, "False", "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable." });
			args.push_back({ mvPyDataType::Bool, "precise_widths", mvArgType::KEYWORD_ARG, "False", "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth." });
			args.push_back({ mvPyDataType::Bool, "no_clip", mvArgType::KEYWORD_ARG, "False", "Disable clipping rectangle for every individual columns." });
			args.push_back({ mvPyDataType::Bool, "pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers." });
			args.push_back({ mvPyDataType::Bool, "no_pad_outerX", mvArgType::KEYWORD_ARG, "False", "Default if BordersOuterV is off. Disable outer-most padding." });
			args.push_back({ mvPyDataType::Bool, "no_pad_innerX", mvArgType::KEYWORD_ARG, "False", "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off)." });
			args.push_back({ mvPyDataType::Bool, "scrollX", mvArgType::KEYWORD_ARG, "False", "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this create a child window, ScrollY is currently generally recommended when using ScrollX." });
			args.push_back({ mvPyDataType::Bool, "scrollY", mvArgType::KEYWORD_ARG, "False", "Enable vertical scrolling." });
			args.push_back({ mvPyDataType::Bool, "no_saved_settings", mvArgType::KEYWORD_ARG, "False", "Never load/save settings in .ini file." });

			mvPythonParserSetup setup;
			setup.about = "Adds a table.";
			setup.category = { "Tables", "Containers", "Widgets" };
			setup.returnType = mvPyDataType::UUID;
			setup.createContextManager = true;

			mvPythonParser parser = FinalizeParser(setup, args);

			parsers->insert({ s_command, parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "column" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Highlight specified table column.";
			setup.category = { "Tables", "App Item Operations"};

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "highlight_table_column", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Unhighlight specified table column.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "unhighlight_table_column", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Set table row color.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_table_row_color", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });

			mvPythonParserSetup setup;
			setup.about = "Remove user set table row color.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "unset_table_row_color", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(4);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::Integer, "column" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Highlight specified table cell.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "highlight_table_cell", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Unhighlight specified table cell.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "unhighlight_table_cell", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::IntList, "color" });

			mvPythonParserSetup setup;
			setup.about = "Highlight specified table row.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "highlight_table_row", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });

			mvPythonParserSetup setup;
			setup.about = "Unhighlight specified table row.";
			setup.category = { "Tables", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "unhighlight_table_row", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Checks if a table column is highlighted.";
			setup.category = { "Tables", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "is_table_column_highlighted", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });

			mvPythonParserSetup setup;
			setup.about = "Checks if a table row is highlighted.";
			setup.category = { "Tables", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "is_table_row_highlighted", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.reserve(3);
			args.push_back({ mvPyDataType::UUID, "table" });
			args.push_back({ mvPyDataType::Integer, "row" });
			args.push_back({ mvPyDataType::Integer, "column" });

			mvPythonParserSetup setup;
			setup.about = "Checks if a table cell is highlighted.";
			setup.category = { "Tables", "App Item Operations" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "is_table_cell_highlighted", parser });
		}
	}

	mvTable::mvTable(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvTable::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvTable*>(item);
		_columns = titem->_columns;
		_inner_width = titem->_inner_width;
		_freezeRows = titem->_freezeRows;
		_freezeColumns = titem->_freezeColumns;
		_flags = titem->_flags;
		_tableHeader = titem->_tableHeader;
	}

	void mvTable::draw(ImDrawList* drawlist, float x, float y)
	{
		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!_show)
			return;

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		if (_enabled)
		{
			if (auto classTheme = getClassThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}
		else
		{
			if (auto classTheme = getClassDisabledThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		{
			ScopedID id(_uuid);

			if (_columns == 0)
				return;

			auto row_renderer = [&](mvAppItem* row)
			{
				ImGui::TableNextRow(0, (float)row->_height);

				row->_state.lastFrameUpdate = GContext->frame;
				row->_state.visible = true;

				//int row_index = ImGui::TableGetRowIndex() + _tableHeader ? 1 : 0;
				int row_index = row->_location;

				if (_rowColorsSet[row_index])
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, _rowColors[row_index]);

				if (_rowSelectionColorsSet[row_index])
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, _rowSelectionColors[row_index]);

				int column_index = -1;
				for (auto& cell : row->_children[1])
				{

					// if tooltip, do not move column index
					if (cell->getType() == mvAppItemType::mvTooltip)
					{
						cell->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
						continue;
					}

					column_index++;

					ImGui::TableSetColumnIndex(column_index);

					if (_columnColorsSet[column_index])
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, _columnColors[column_index]);

					if (_cellColorsSet[row_index][column_index])
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, _cellColors[row_index][column_index]);

					cell->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
				}
			};

			if (ImGui::BeginTable(_internalLabel.c_str(), _columns, _flags,
				ImVec2((float)_width, (float)_height), (float)_inner_width))
			{
				_state.lastFrameUpdate = GContext->frame;
				_state.visible = true;

				ImGui::TableSetupScrollFreeze(_freezeRows, _freezeColumns);

				// columns
				for (auto& item : _children[0])
				{
					// skip item if it's not shown
					if (!item->_show)
						continue;

					item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
				}

				if (_tableHeader)
					ImGui::TableHeadersRow();

				if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
				{
					if (sorts_specs->SpecsDirty)
					{
						if (sorts_specs->SpecsCount == 0)
							mvAddCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
						else
						{

							// generate id map for columns
							std::unordered_map<ImGuiID, mvUUID> idMap;
							for (size_t i = 0; i < _children[0].size(); i++)
								idMap[static_cast<mvTableColumn*>(_children[0][i].get())->_id] = _children[0][i]->_uuid;

							std::vector<SortSpec> specs;
							for (int i = 0; i < sorts_specs->SpecsCount; i++)
							{
								const ImGuiTableColumnSortSpecs* sort_spec = &sorts_specs->Specs[i];
								specs.push_back({ idMap[sort_spec->ColumnUserID], sort_spec->SortDirection == ImGuiSortDirection_Ascending ? 1 : -1 });
							}

							mvSubmitCallback([=]() {
								PyObject* pySpec = PyList_New(specs.size());
								for (size_t i = 0; i < specs.size(); i++)
								{
									PyObject* pySingleSpec = PyList_New(2);
									PyList_SetItem(pySingleSpec, 0, ToPyLong(specs[i].column));
									PyList_SetItem(pySingleSpec, 1, ToPyInt(specs[i].direction));
									PyList_SetItem(pySpec, i, pySingleSpec);
								}

								if (_alias.empty())
									mvRunCallback(getCallback(false), _uuid, pySpec, _user_data);
								else
									mvRunCallback(getCallback(false), _alias, pySpec, _user_data);
								Py_XDECREF(pySpec);
								});
						}
						sorts_specs->SpecsDirty = false;
					}
				}

				if (_rows != 0)
				{

					if (_imguiFilter.IsActive())
					{
						for (auto& row : _children[1])
						{
							if (!_imguiFilter.PassFilter(row->_filter.c_str()))
								continue;
							row_renderer(row.get());
						}
					}

					else if (_useClipper)
					{
						ImGuiListClipper clipper;
						clipper.Begin((int)_children[1].size());

						while (clipper.Step())
						{
							for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
								row_renderer(_children[1][row_n].get());

						}
						clipper.End();
					}
					else
					{
						for (auto& row : _children[1])
							row_renderer(row.get());
					}
				}

				// columns
				int columnnum = 0;
				for (auto& item : _children[0])
				{
					ImGuiTableColumnFlags flags = ImGui::TableGetColumnFlags(columnnum);
					item->_state.lastFrameUpdate = GContext->frame;
					item->_state.visible = flags & ImGuiTableColumnFlags_IsVisible;
					item->_state.hovered = flags & ImGuiTableColumnFlags_IsHovered;
					columnnum++;
				}

				ImGui::EndTable();
			}
		}

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		if (_enabled)
		{
			if (auto classTheme = getClassThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->customAction();
		}
		else
		{
			if (auto classTheme = getClassDisabledThemeComponent())
				static_cast<mvThemeComponent*>(classTheme.get())->customAction();
		}

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

	}

	void mvTable::onChildAdd(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvTableColumn)
		{
			_columns++;
			_columnColors.push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
			_columnColorsSet.push_back(false);
		}

		else if (item->getType() == mvAppItemType::mvTableRow)
		{
			_rows++;
			_rowColors.push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
			_rowColorsSet.push_back(false);
			_rowSelectionColors.push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
			_rowSelectionColorsSet.push_back(false);
			
			_cellColorsSet.push_back({});
			_cellColors.push_back({});
			for (int i = 0; i < _columns; i++)
			{
				_cellColorsSet.back().push_back(false);
				_cellColors.back().push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
			}
		}
	}

	void mvTable::onChildRemoved(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvTableColumn)
			_columns--;
		else if (item->getType() == mvAppItemType::mvTableRow)
			_rows--;


		_columnColors.resize(_columns);
		_columnColorsSet.resize(_columns);
		_rowColors.resize(_rows);
		_rowColorsSet.resize(_rows);
		_rowSelectionColors.resize(_rows);
		_rowSelectionColorsSet.resize(_rows);

		_cellColorsSet.resize(_rows);
		_cellColors.resize(_rows);
		for(int i = 0; i<_columns; i++)
		{
			_columnColors[i] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			_columnColorsSet[i] = false;
		}

		for (int i = 0; i < _rows; i++)
		{
			_rowColors[i] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			_rowColorsSet[i] = false;
			_rowSelectionColors[i] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			_rowSelectionColorsSet[i] = false;

			_cellColorsSet[i].resize(_columns);
			_cellColors[i].resize(_columns);
			for (int j = 0; j < _columns; j++)
			{
				_cellColorsSet[i][j] = false;
				_cellColors[i][j] = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
	}

	void mvTable::onChildrenRemoved()
	{
		_columns = (int)_children[0].size();
		_rows = (int)_children[1].size();

		_columnColors.clear();
		_columnColorsSet.clear();
		_rowColors.clear();
		_rowColorsSet.clear();
		_rowSelectionColors.clear();
		_rowSelectionColorsSet.clear();

		_columnColors.shrink_to_fit();
		_columnColorsSet.shrink_to_fit();
		_rowColors.shrink_to_fit();
		_rowColorsSet.shrink_to_fit();
		_rowSelectionColors.shrink_to_fit();
		_rowSelectionColorsSet.shrink_to_fit();

		for (int i = 0; i < _columns; i++)
		{
			_columnColors.push_back(ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)));
			_columnColorsSet.push_back(false);
		}
	}

	void mvTable::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "freeze_rows")) _freezeRows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "freeze_columns")) _freezeColumns = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "header_row")) _tableHeader = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "clipper")) _useClipper = ToBool(item);
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
		mvPyObject py_clipper = ToPyBool(_useClipper);

		PyDict_SetItemString(dict, "freeze_rows", py_freeze_rows);
		PyDict_SetItemString(dict, "freeze_columns", py_freeze_columns);
		PyDict_SetItemString(dict, "inner_width", py_inner_width);
		PyDict_SetItemString(dict, "header_row", py_header_row);
		PyDict_SetItemString(dict, "clipper", py_clipper);

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

	PyObject* mvTable::getPyValue()
	{
		return ToPyString(std::string(_imguiFilter.InputBuf));
	}

	void mvTable::setPyValue(PyObject* value)
	{
		auto str_value = ToString(value);

		int i = 0;
		for (auto& character : str_value)
		{
			if (i > 254)
				break;

			_imguiFilter.InputBuf[i] = character;

			++i;
		}
		_imguiFilter.InputBuf[i] = 0;
		_imguiFilter.Build();
	}

	PyObject* mvTable::highlight_table_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int column = 0;
		PyObject* color;

		if (!Parse((GetParsers())["highlight_table_column"], args, kwargs, __FUNCTION__, &tableraw, &column, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "highlight_table_column",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "highlight_table_column",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (column >= tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "highlight_table_column",
				"Column out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_columnColorsSet[column] = true;
		tablecast->_columnColors[column] = finalColor;

		return GetPyNone();
	}

	PyObject* mvTable::unhighlight_table_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int column = 0;

		if (!Parse((GetParsers())["unhighlight_table_column"], args, kwargs, __FUNCTION__, &tableraw, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unhighlight_table_column",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unhighlight_table_column",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (column >= tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "unhighlight_table_column",
				"Column out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_columnColorsSet[column] = false;

		return GetPyNone();
	}

	PyObject* mvTable::set_table_row_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		PyObject* color;

		if (!Parse((GetParsers())["set_table_row_color"], args, kwargs, __FUNCTION__, &tableraw, &row, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_table_row_color",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_table_row_color",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "set_table_row_color",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_rowColorsSet[row] = finalColor.a >= 0.0f;
		tablecast->_rowColors[row] = finalColor;

		return GetPyNone();
	}

	PyObject* mvTable::unset_table_row_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;

		if (!Parse((GetParsers())["unset_table_row_color"], args, kwargs, __FUNCTION__, &tableraw, &row))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unset_table_row_color",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unset_table_row_color",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "set_table_row_color",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_rowColorsSet[row] = false;
		return GetPyNone();
	}

	PyObject* mvTable::highlight_table_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		PyObject* color;

		if (!Parse((GetParsers())["highlight_table_row"], args, kwargs, __FUNCTION__, &tableraw, &row, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "highlight_table_row",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "highlight_table_row",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "highlight_table_row",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_rowSelectionColorsSet[row] = true;
		tablecast->_rowSelectionColors[row] = finalColor;

		return GetPyNone();
	}

	PyObject* mvTable::unhighlight_table_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;

		if (!Parse((GetParsers())["unhighlight_table_row"], args, kwargs, __FUNCTION__, &tableraw, &row))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unhighlight_table_row",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unhighlight_table_row",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "unselect_table_row",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_rowSelectionColorsSet[row] = false;

		return GetPyNone();
	}

	PyObject* mvTable::highlight_table_cell(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		int column = 0;
		PyObject* color;

		if (!Parse((GetParsers())["highlight_table_cell"], args, kwargs, __FUNCTION__, &tableraw, &row, &column, &color))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "highlight_table_cell",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "highlight_table_cell",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows || column > tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "highlight_table_cell",
				"Row/Column out of range", tablecast);
			return GetPyNone();
		}

		mvColor finalColor = ToColor(color);
		tablecast->_cellColorsSet[row][column] = true;
		tablecast->_cellColors[row][column] = finalColor;

		return GetPyNone();
	}

	PyObject* mvTable::unhighlight_table_cell(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		int column = 0;

		if (!Parse((GetParsers())["unhighlight_table_cell"], args, kwargs, __FUNCTION__, &tableraw, &row, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unhighlight_table_cell",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "unhighlight_table_cell",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows || column > tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "unhighlight_table_cell",
				"Row/Column out of range", tablecast);
			return GetPyNone();
		}

		tablecast->_cellColorsSet[row][column] = false;

		return GetPyNone();
	}

	PyObject* mvTable::is_table_cell_highlighted(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;
		int column = 0;

		if (!Parse((GetParsers())["is_table_cell_highlighted"], args, kwargs, __FUNCTION__, &tableraw, &row, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_table_cell_highlighted",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_table_cell_highlighted",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows || column > tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "is_table_cell_highlighted",
				"Row/Column out of range", tablecast);
			return GetPyNone();
		}

		if (tablecast->_cellColorsSet[row][column])
			return ToPyBool(true);
		if (tablecast->_rowColorsSet[row])
			return ToPyBool(true);
		if (tablecast->_columnColorsSet[column])
			return ToPyBool(true);

		return ToPyBool(false);
	}

	PyObject* mvTable::is_table_row_highlighted(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int row = 0;

		if (!Parse((GetParsers())["is_table_row_highlighted"], args, kwargs, __FUNCTION__, &tableraw, &row))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_table_row_highlighted",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_table_row_highlighted",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (row >= tablecast->_rows)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "is_table_row_highlighted",
				"Row out of range", tablecast);
			return GetPyNone();
		}

		return ToPyBool(tablecast->_rowSelectionColorsSet[row]);
	}

	PyObject* mvTable::is_table_column_highlighted(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* tableraw;
		int column = 0;

		if (!Parse((GetParsers())["is_table_column_highlighted"], args, kwargs, __FUNCTION__, &tableraw, &column))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID table = GetIDFromPyObject(tableraw);

		auto atable = GetItem(*GContext->itemRegistry, table);
		if (atable == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "is_table_column_highlighted",
				"Item not found: " + std::to_string(table), nullptr);
			return GetPyNone();
		}

		if (atable->getType() != mvAppItemType::mvTable)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "is_table_column_highlighted",
				"Incompatible type. Expected types include: mvTable", atable);
			return GetPyNone();
		}

		mvTable* tablecast = static_cast<mvTable*>(atable);

		if (column >= tablecast->_columns)
		{
			mvThrowPythonError(mvErrorCode::mvNone, "is_table_column_highlighted",
				"Column out of range", tablecast);
			return GetPyNone();
		}

		return ToPyBool(tablecast->_columnColorsSet[column]);
	}
}