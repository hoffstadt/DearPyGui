#include <utility>
#include "mvCombo.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvCombo::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a combo dropdown that allows a user to select a single option from a drop down window.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_ENABLED |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::StringList>("items", mvArgType::POSITIONAL_ARG, "()", "A tuple of items to be shown in the drop down window. Can consist of any combination of types.");

		parser.addArg<mvPyDataType::String>("default_value", mvArgType::KEYWORD_ARG, "''");
		parser.addArg<mvPyDataType::Bool>("popup_align_left", mvArgType::KEYWORD_ARG, "False", "Align the popup toward the left.");
		parser.addArg<mvPyDataType::Bool>("no_arrow_button", mvArgType::KEYWORD_ARG, "False", "Display the preview box without the square arrow button.");
		parser.addArg<mvPyDataType::Bool>("no_preview", mvArgType::KEYWORD_ARG, "False", "Display only the square arrow button.");

		parser.addArg<mvPyDataType::Long>("height_mode", mvArgType::KEYWORD_ARG, "1", "mvComboHeight_Small, _Regular, _Large, _Largest");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvCombo::mvCombo(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	PyObject* mvCombo::getPyValue()
	{
		return ToPyString(*_value);
	}

	void mvCombo::setPyValue(PyObject* value)
	{
		*_value = ToString(value);
	}

	void mvCombo::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::string>*>(item->getValue());
	}

	void mvCombo::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id(_uuid);

		static std::vector<std::string> disabled_items{};

		// The second parameter is the label previewed before opening the combo.
		if (ImGui::BeginCombo(_internalLabel.c_str(), _value->c_str(), _flags)) 
		{
			for (const auto& name : _enabled ? _items : disabled_items)
			{
				bool is_selected = (*_value == name);
				if (ImGui::Selectable((name).c_str(), is_selected))
				{
					if (_enabled) { *_value = name; }
	
					auto value = *_value;
					mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
						mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyString(value), _user_data);
						});


				}

				// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				if (is_selected)
					ImGui::SetItemDefaultFocus();   
			}

			ImGui::EndCombo();
		}

	}

	void mvCombo::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_items = ToStringVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvCombo::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "items")) _items = ToStringVect(item);

		if (PyObject* item = PyDict_GetItemString(dict, "height_mode"))
		{
			long height_mode = ToUUID(item);

			if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Small)
				_flags = ImGuiComboFlags_HeightSmall;
			else if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Regular)
				_flags = ImGuiComboFlags_HeightRegular;
			else if (height_mode == (long)mvCombo::ComboHeightMode::mvComboHeight_Large)
				_flags = ImGuiComboFlags_HeightLarge;
			else
				_flags = ImGuiComboFlags_HeightLargest;
		}

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, _flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, _flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, _flags);

	}

	void mvCombo::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "items", ToPyList(_items));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};
		checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, _flags);
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, _flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, _flags);

		if (_flags & ImGuiComboFlags_HeightSmall)
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Small));
		else if (_flags & ImGuiComboFlags_HeightRegular)
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Regular));
		else if (_flags & ImGuiComboFlags_HeightLarge)
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Large));
		else
			PyDict_SetItemString(dict, "height_mode", ToPyUUID((long)mvCombo::ComboHeightMode::mvComboHeight_Largest));

	}

}