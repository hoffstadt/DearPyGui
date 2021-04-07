#include <utility>
#include "mvAnnotation.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvAnnotation::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "default_value", "", "(0.0, 0.0)"},
			{mvPythonDataType::FloatList, "offset", "", "(0.0, 0.0)"},
			{mvPythonDataType::String, "label", "Overrides 'name' as label", "''"},
			{mvPythonDataType::String, "source", "", "''"},
			{mvPythonDataType::FloatList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Bool, "clamped", "", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a drag point to a plot.", "None", "Plotting") });
	}

	mvAnnotation::mvAnnotation(const std::string& name)
		: mvFloat4PtrBase(name)
	{
	}

	bool mvAnnotation::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvAnnotation::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvFontScope fscope(this);

		if (m_clamped)
			ImPlot::AnnotateClamped((*m_value.get())[0], (*m_value.get())[1], m_pixOffset, m_color.toVec4(), m_label.c_str());
		else
			ImPlot::Annotate((*m_value.get())[0], (*m_value.get())[1], m_pixOffset, m_color.toVec4(), m_label.c_str());

	}

	void mvAnnotation::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "clamped")) m_clamped = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "offset")) m_pixOffset = ToVec2(item);

	}

	void mvAnnotation::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}