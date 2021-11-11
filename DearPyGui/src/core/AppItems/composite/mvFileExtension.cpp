#include "mvFileExtension.h"
#include "mvFileDialog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvFileExtension::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			std::vector<mvPythonDataElement> args;

			AddCommonArgs(args,(CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE)
			);

			args.push_back({ mvPyDataType::String, "extension", mvArgType::REQUIRED_ARG, "*", "Extension that will show as an when the parent is a file dialog." });
			args.push_back({ mvPyDataType::String, "custom_text", mvArgType::KEYWORD_ARG, "''", "Replaces the displayed text in the drop down for this extension." });
			args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(-255, 0, 0, 255)", "Color for the text that will be shown with specified extensions."});

			mvPythonParserSetup setup;
			setup.about = "Creates a file extension filter option in the file dialog.";
			setup.category = {"File Dialog"};
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ s_command, parser });
		}

	}

	mvFileExtension::mvFileExtension(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	void mvFileExtension::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvFileExtension*>(item);
		_customText = titem->_customText;
		_extension = titem->_extension;
	}

	void mvFileExtension::draw(ImDrawList* drawlist, float x, float y)
	{
		IGFD::FileExtentionInfos info;
		info.icon = _customText;
		if (_color.r >= 0)
			info.color = _color.toVec4();
		else
			info.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		static_cast<mvFileDialog*>(_parentPtr)->getDialog().SetExtentionInfos(_extension.c_str(), info);
	}

	void mvFileExtension::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_extension = ToString(item);
				break;

			default:
				break;
			}
		}
	}

	void mvFileExtension::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "custom_text"))_customText = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);

		if (_parentPtr)
			static_cast<mvFileDialog*>(_parentPtr)->markDirty();
	}

	void mvFileExtension::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "custom_text", mvPyObject(ToPyString(_customText)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	}

}