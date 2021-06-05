#include <utility>
#include "mvDragLine.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDragLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Plotting", "Widgets" });
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
		parser.addArg<mvPyDataType::Bool>("vertical", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDragLine::mvDragLine(const std::string& name)
		: mvDoublePtrBase(name)
	{
	}

	bool mvDragLine::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvDragLine::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvFontScope fscope(this);

		

		if (m_vertical)
		{
			if (ImPlot::DragLineX(m_specificedlabel.c_str(), m_value.get(), m_show_label, m_color, m_thickness))
			{
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr, nullptr);
			}
		}
		else
		{
			if (ImPlot::DragLineY(m_specificedlabel.c_str(), m_value.get(), m_show_label, m_color, m_thickness))
			{
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr, nullptr);
			}
		}

	}

	void mvDragLine::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) m_show_label = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);

	}

	void mvDragLine::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "show_label", ToPyBool(m_show_label));
		PyDict_SetItemString(dict, "vertical", ToPyBool(m_vertical));
	}

}