#include "mvFileExtension.h"
#include "mvFileDialog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvFileExtension::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::UUID, "Creates a file extension filter option in the file dialog. Only works when the parent is a file dialog.", { "File Dialog"});
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE)
			);

			parser.addArg<mvPyDataType::String>("extension", mvArgType::REQUIRED_ARG, "*", "Extension that will show as an when the parent is a file dialog.");
			parser.addArg<mvPyDataType::String>("custom_text", mvArgType::KEYWORD_ARG, "''", "Replaces the displayed text in the drop down for this extension.");
			parser.addArg<mvPyDataType::FloatList>("color", mvArgType::KEYWORD_ARG, "(-255, 0, 0, 255)");

			//parser.addArg<mvPyDataType::Callable>("callback", mvArgType::KEYWORD_ARG, "None", "function to call on completion");
			parser.finalize();
			parsers->insert({ s_command, parser });
		}

	}

	mvFileExtension::mvFileExtension(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{
	}

	bool mvFileExtension::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvFileDialog) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: file dialog, staging container.", this);
		MV_ITEM_REGISTRY_ERROR("File dialog extension parent must be a file dialog.");
		assert(false);
		return false;
	}

	void mvFileExtension::draw(ImDrawList* drawlist, float x, float y)
	{
		IGFD::FileExtentionInfosStruct info;
		info.icon = m_customText.c_str();
		if (m_color.r >= 0)
			info.color = m_color.toVec4();
		else
			info.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		static_cast<mvFileDialog*>(m_parentPtr)->getDialog().SetExtentionInfos(m_extension.c_str(), info);
	}

	void mvFileExtension::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_extension = ToString(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "custom_text"))m_customText = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);

		if (m_parentPtr)
			static_cast<mvFileDialog*>(m_parentPtr)->markDirty();
	}

	void mvFileExtension::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "custom_text", ToPyString(m_customText));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
	}

}