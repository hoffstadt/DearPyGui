#include <utility>
#include "mvDragPoint.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDragPoint::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0, 0.0, 0.0)");


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
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvDragPoint::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvFontScope fscope(this);

		static double dummyx = (*m_value.get())[0];
		static double dummyy = (*m_value.get())[1];
		dummyx = (*m_value.get())[0];
		dummyy = (*m_value.get())[1];

		if (ImPlot::DragPoint(m_specificedlabel.c_str(), &dummyx, &dummyy, m_show_label, m_color, m_radius))
		{
			(*m_value.get())[0] = dummyx;
			(*m_value.get())[1] = dummyy;
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_uuid, nullptr, nullptr);
		}

	}

	void mvDragPoint::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) m_show_label = ToBool(item);

	}

	void mvDragPoint::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "radius", ToPyFloat(m_radius));
		PyDict_SetItemString(dict, "show_label", ToPyBool(m_show_label));
	}

}