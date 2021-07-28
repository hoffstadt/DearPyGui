#include <utility>
#include "mvDragLine.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDragLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a drag line to a plot.", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("default_value", mvArgType::KEYWORD_ARG, "0.0");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::Bool>("show_label", mvArgType::KEYWORD_ARG, "True");
		parser.addArg<mvPyDataType::Bool>("vertical", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDragLine::mvDragLine(mvUUID uuid)
		: mvDoublePtrBase(uuid)
	{
	}

	bool mvDragLine::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot, staging container", this);
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvDragLine::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);
		//mvFontScope fscope(this);

		

		if (_vertical)
		{
			if (ImPlot::DragLineX(_specificedlabel.c_str(), _value.get(), _show_label, _color, _thickness))
			{
				mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, nullptr, _user_data);
			}
		}
		else
		{
			if (ImPlot::DragLineY(_specificedlabel.c_str(), _value.get(), _show_label, _color, _thickness))
			{
				mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, nullptr, _user_data);
			}
		}

	}

	void mvDragLine::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) _show_label = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) _vertical = ToBool(item);

	}

	void mvDragLine::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(_thickness));
		PyDict_SetItemString(dict, "show_label", ToPyBool(_show_label));
		PyDict_SetItemString(dict, "vertical", ToPyBool(_vertical));
	}

}