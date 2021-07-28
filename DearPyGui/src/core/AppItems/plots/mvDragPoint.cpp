#include <utility>
#include "mvDragPoint.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDragPoint::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a drag point to a plot.", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");


		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");


		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::Bool>("show_label", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDragPoint::mvDragPoint(mvUUID uuid)
		: mvDouble4PtrBase(uuid)
	{
	}

	bool mvDragPoint::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot, staging container", this);
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvDragPoint::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		static double dummyx = (*_value.get())[0];
		static double dummyy = (*_value.get())[1];
		dummyx = (*_value.get())[0];
		dummyy = (*_value.get())[1];

		if (ImPlot::DragPoint(_specificedlabel.c_str(), &dummyx, &dummyy, _show_label, _color, _radius))
		{
			(*_value.get())[0] = dummyx;
			(*_value.get())[1] = dummyy;
			mvApp::GetApp()->getCallbackRegistry().addCallback(_callback, _uuid, nullptr, _user_data);
		}

	}

	void mvDragPoint::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) _show_label = ToBool(item);

	}

	void mvDragPoint::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "radius", ToPyFloat(_radius));
		PyDict_SetItemString(dict, "show_label", ToPyBool(_show_label));
	}

}