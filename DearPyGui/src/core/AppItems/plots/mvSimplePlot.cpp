#include "mvSimplePlot.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvSimplePlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::FloatList, "default_value", mvArgType::KEYWORD_ARG, "()" });
		args.push_back({ mvPyDataType::String, "overlay", mvArgType::KEYWORD_ARG, "''", "overlays text (similar to a plot title)" });
		args.push_back({ mvPyDataType::Bool, "histogram", mvArgType::KEYWORD_ARG, "False" });
		args.push_back({ mvPyDataType::Bool, "autosize", mvArgType::KEYWORD_ARG, "True" });	
		args.push_back({ mvPyDataType::Float, "min_scale", mvArgType::KEYWORD_ARG, "0.0" });
		args.push_back({ mvPyDataType::Float, "max_scale", mvArgType::KEYWORD_ARG, "0.0" });

		mvPythonParserSetup setup;
		setup.about = "Adds a simple plot for visualization of a 1 dimensional set of values.";
		setup.category = { "Plotting", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvSimplePlot::mvSimplePlot(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvSimplePlot::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvSimplePlot*>(item);
		if(_source != 0) _value = titem->_value;
		_overlay = titem->_overlay;
		_min = titem->_min;
		_max = titem->_max;
		_histogram = titem->_histogram;
		_autosize = titem->_autosize;
	}

	void mvSimplePlot::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!_show)
			return;

		// focusing
		if (_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			_focusNextFrame = false;
		}

		// cache old cursor position
		ImVec2 previousCursorPos = ImGui::GetCursorPos();

		// set cursor position if user set
		if (_dirtyPos)
			ImGui::SetCursorPos(_state.pos);

		// update widget's position state
		_state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		// set indent
		if (_indent > 0.0f)
			ImGui::Indent(_indent);

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{

			ImGui::PushID(this);

			if (_histogram)
				ImGui::PlotHistogram(_internalLabel.c_str(), _value->data(), (int)_value->size(), 0, _overlay.c_str(),
					_min, _max, ImVec2((float)_width, (float)_height));
			else
				ImGui::PlotLines(_internalLabel.c_str(), _value->data(), (int)_value->size(), 0, _overlay.c_str(),
					_min, _max, ImVec2((float)_width, (float)_height));

			ImGui::PopID();
		}

		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------
		_state.lastFrameUpdate = GContext->frame;
		_state.hovered = ImGui::IsItemHovered();
		_state.leftclicked = ImGui::IsItemClicked();
		_state.rightclicked = ImGui::IsItemClicked(1);
		_state.middleclicked = ImGui::IsItemClicked(2);
		_state.visible = ImGui::IsItemVisible();
		_state.rectMin = { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
		_state.rectMax = { ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y };
		_state.rectSize = { ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y };
		_state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// set cursor position to cached position
		if (_dirtyPos)
			ImGui::SetCursorPos(previousCursorPos);

		if (_indent > 0.0f)
			ImGui::Unindent(_indent);

		// pop font off stack
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);

		// handle drag & drop payloads
		for (auto& item : _children[3])
			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		// handle drag & drop if used
		if (_dropCallback)
		{
			ScopedID id(_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if (_alias.empty())
						mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
					else
						mvAddCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	PyObject* mvSimplePlot::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvSimplePlot::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<float>>*>(item->getValue());
	}

	void mvSimplePlot::setPyValue(PyObject* value)
	{
		*_value = ToFloatVect(value);

		if (!_autosize)
			return;
		if (!_value->empty())
		{
			_max = _value->data()[0];
			_min = _value->data()[0];

			for (auto& item : *_value)
			{
				if (item > _max)
					_max = item;
				if (item < _min)
					_min = item;
			}
		}
	}

	void mvSimplePlot::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) _overlay = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "minscale")) _min = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "maxscale")) _max = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "histogram")) _histogram = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize")) _autosize = ToBool(item);
	}

	void mvSimplePlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "overlay", mvPyObject(ToPyString(_overlay)));
		PyDict_SetItemString(dict, "minscale", mvPyObject(ToPyFloat(_min)));
		PyDict_SetItemString(dict, "maxscale", mvPyObject(ToPyFloat(_max)));
		PyDict_SetItemString(dict, "histogram", mvPyObject(ToPyBool(_histogram)));
		PyDict_SetItemString(dict, "autosize", mvPyObject(ToPyBool(_autosize)));
	}

}