#include "mvSpacing.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvSpacing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds vertical spacing.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Integer>("count", mvArgType::KEYWORD_ARG, "1", "Number of spacings to add the size is dependant on the curret style.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvSpacing::mvSpacing(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvSpacing::setDataSource(mvUUID dataSource)
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
		_value = std::get<std::shared_ptr<int>>(item->getValue());
	}

	PyObject* mvSpacing::getPyValue()
	{
		return ToPyInt(*_value);
	}

	void mvSpacing::setPyValue(PyObject* value)
	{
		*_value = ToInt(value);
	}

	void mvSpacing::draw(ImDrawList* drawlist, float x, float y)
	{
		for (int i = 0; i < *_value; i++)
			ImGui::Spacing();
	}

	void mvSpacing::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "count"))
		{
			*_value = ToInt(item);
		}


	}

	void mvSpacing::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "count", ToPyBool(*_value));
	}

}