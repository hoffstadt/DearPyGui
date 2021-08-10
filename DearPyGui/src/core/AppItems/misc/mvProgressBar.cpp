#include <utility>
#include "mvProgressBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvProgressBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a progress bar.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::String>("overlay", mvArgType::KEYWORD_ARG, "''", "Overlayed text.");
		parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0", "Normalized value to fill the bar from 0.0 to 1.0.");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvProgressBar::mvProgressBar(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvProgressBar::getPyValue()
	{
		return ToPyFloat(*_value);
	}

	void mvProgressBar::setPyValue(PyObject* value)
	{
		*_value = ToFloat(value);
	}

	void mvProgressBar::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<float>*>(item->getValue());
	}

	void mvProgressBar::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		ImGui::ProgressBar(*_value, ImVec2((float)_width, (float)_height), _overlay.c_str());

	}

	void mvProgressBar::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) _overlay = ToString(item);
	}

	void mvProgressBar::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "overlay", ToPyString(_overlay));
	}

}