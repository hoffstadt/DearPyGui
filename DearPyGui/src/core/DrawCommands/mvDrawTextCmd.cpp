#include "mvDrawTextCmd.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvDrawTextCmd::mvDrawTextCmd(const mvVec2& pos, std::string  text, const mvColor& color, int size)
		:
		m_pos(pos),
		m_text(std::move(text)),
		m_color(color),
		m_size(size)
	{
	}

	void mvDrawTextCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddText(ImGui::GetFont(), (float)m_size, m_pos + start, m_color, m_text.c_str());
	}

	void mvDrawTextCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "text")) m_text = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pos")) m_pos = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToInt(item);

	}

	void mvDrawTextCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "text", ToPyString(m_text));
		PyDict_SetItemString(dict, "pos", ToPyPair(m_pos.x, m_pos.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "size", ToPyInt(m_size));
	}
}