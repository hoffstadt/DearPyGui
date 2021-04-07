#include <utility>
#include "mvDragPoint.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvDragPoint::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0, 0.0, 0.0)"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "radius", "", "4.0"},
			{mvPythonDataType::Bool, "show_label", "", "True"},
			{mvPythonDataType::Callable, "callback", "function to run when point is moved", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvDragPoint::mvDragPoint(const std::string& name)
		: mvFloat4PtrBase(name)
	{
	}

	bool mvDragPoint::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvDragPoint::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvFontScope fscope(this);

		static double dummyx = (*m_value.get())[0];
		static double dummyy = (*m_value.get())[1];
		dummyx = (*m_value.get())[0];
		dummyy = (*m_value.get())[1];

		if (ImPlot::DragPoint(m_label.c_str(), &dummyx, &dummyy, m_show_label, m_color, m_radius))
		{
			(*m_value.get())[0] = dummyx;
			(*m_value.get())[1] = dummyy;
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, nullptr);
		}

	}

	void mvDragPoint::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) m_show_label = ToBool(item);

	}

	void mvDragPoint::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}