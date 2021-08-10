#include "mvTheme.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTheme::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Themes", "Containers"}, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.addArg<mvPyDataType::Bool>("default_theme", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });

	}

	mvTheme::mvTheme(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = false;
	}

	void mvTheme::draw(ImDrawList* drawlist, float x, float y)
	{
		for (auto& childset : _children)
		{
			for (auto& child : childset)
				child->draw(drawlist, x, y);
		}
	}

	void mvTheme::customAction()
	{
		for (auto& childset : _children)
		{
			for (auto& child : childset)
				child->customAction();
		}
	}

	void mvTheme::alternativeCustomAction()
	{
		if (!_default_theme)
		{
			_triggerAlternativeAction = false;
			return;
		}

		for (auto& childset : _children)
		{
			for (auto& child : childset)
				child->alternativeCustomAction();
		}
		_triggerAlternativeAction = false;
	}

	void mvTheme::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "default_theme"))
		{
			_default_theme = ToBool(item);
			_triggerAlternativeAction = ToBool(item);
		}

	}

}
