#include <utility>
#include "mvDragLine.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvDragLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::Bool, "show_label", "", "True"},
			{mvPythonDataType::Bool, "vertical", "", "True"},
			{mvPythonDataType::Callable, "callback", "function to run when point is moved", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvDragLine::mvDragLine(const std::string& name, float default_value, const std::string& dataSource)
		: mvFloatPtrBase(name, default_value)
	{
	}

	bool mvDragLine::isParentCompatible(mvAppItemType type)
	{
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

		static double dummy = *(float*)m_value.get();
		dummy = *(float*)m_value.get();

		if (m_vertical)
		{
			if (ImPlot::DragLineX(m_label.c_str(), &dummy, m_show_label, m_color, m_thickness))
			{
				*m_value.get() = (float)dummy;
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr);
			}
		}
		else
		{
			if (ImPlot::DragLineY(m_label.c_str(), &dummy, m_show_label, m_color, m_thickness))
			{
				*m_value.get() = (float)dummy;
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr);
			}
		}

	}

	void mvDragLine::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) m_show_label = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);

	}

	void mvDragLine::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}


	PyObject* mvDragLine::add_drag_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();

		float default_value = 0.0f;

		const char* label = "";
		const char* source = "";

		PyObject* color = PyTuple_New(4);
		PyTuple_SetItem(color, 0, PyLong_FromLong(0));
		PyTuple_SetItem(color, 1, PyLong_FromLong(0));
		PyTuple_SetItem(color, 2, PyLong_FromLong(0));
		PyTuple_SetItem(color, 3, PyLong_FromLong(-1));

		float thickness = 1.0f;

		int show_label = true;
		int vertical = true;

		PyObject* callback = nullptr;
		const char* before = "";
		const char* parent = "";
		
		int show = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__, 
			&name, &default_value, &label, &source, &color, &thickness, &show_label, &vertical,
			&callback,
			&parent, &before, &show))
			return GetPyNone();

		auto item = CreateRef<mvDragLine>(name, default_value, source);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}