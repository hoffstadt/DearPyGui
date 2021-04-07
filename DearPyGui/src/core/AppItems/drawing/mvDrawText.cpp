#include "mvDrawText.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawText::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "pos"},
			{mvPythonDataType::String, "text"},
			{mvPythonDataType::IntList, "color", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Integer, "size", "", "10"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws text on a drawing.", "None", "Drawing") });
	}

	mvDrawText::mvDrawText(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	bool mvDrawText::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawText::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };
		drawlist->AddText(ImGui::GetFont(), (float)m_size, m_pos + start, m_color, m_text.c_str());
	}

	void mvDrawText::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "text")) m_text = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pos")) m_pos = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToInt(item);

	}

	void mvDrawText::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "text", ToPyString(m_text));
		PyDict_SetItemString(dict, "pos", ToPyPair(m_pos.x, m_pos.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "size", ToPyInt(m_size));
	}
}