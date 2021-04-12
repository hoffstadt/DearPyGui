#include "mvNode.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImNodesThemeScope.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	static std::string FindRenderedTextEnd(const char* text, const char* text_end = nullptr)
	{
		int size = 0;

		const char* text_display_end = text;
		if (!text_end)
			text_end = (const char*)-1;

		while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		{
			text_display_end++;
			size++;
		}

		char* cvalue = new char[size + 1];
		for (int i = 0; i < size; i++)
			cvalue[i] = text[i];

		cvalue[size] = '\0';

		auto result = std::string(cvalue);
		delete[] cvalue;

		return result;
	}

	void mvNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("draggable", mvArgType::KEYWORD, "True");

		parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD, "100");
		parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD, "100");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNode::mvNode(const std::string& name)
		: mvAppItem(name)
	{
		m_label = FindRenderedTextEnd(m_name.c_str());
		m_specificedlabel = m_label;
        int64_t address = (int64_t)this;
        int64_t reduced_address = address % 2147483648;
        m_id = (int)reduced_address;
	}

	bool mvNode::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvNodeEditor)
			return true;

		mvThrowPythonError(1000, "Node parent must be node editor.");
		MV_ITEM_REGISTRY_ERROR("Node parent must be node editor.");
		assert(false);
		return false;
	}

	bool mvNode::canChildBeAdded(mvAppItemType type)
	{
		if(type == mvAppItemType::mvNodeAttribute)
			return true;

		mvThrowPythonError(1000, "Node children must be node attributes only.");
		MV_ITEM_REGISTRY_ERROR("Node children must be node attributes only.");
		assert(false);

		return false;
	}

	void mvNode::setNodePos(float x, float y)
	{
		m_xpos = (int)x;
		m_ypos = (int)y;
		m_dirty_pos = true;
	}

	void mvNode::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImNodesThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_dirty_pos)
		{
			imnodes::SetNodeGridSpacePos((int)m_id, ImVec2(m_xpos, m_ypos));
			m_dirty_pos = false;
		}

		imnodes::SetNodeDraggable((int)m_id, m_draggable);

		imnodes::BeginNode(m_id);

		imnodes::BeginNodeTitleBar();
		ImGui::TextUnformatted(m_label.c_str());
		imnodes::EndNodeTitleBar();

		//we do this so that the children dont get the theme
		scope.cleanup();

		for (auto item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			item->draw(drawlist, x, y);

			auto& state = item->getState();
			state.setActive(imnodes::IsAttributeActive());

		}

		imnodes::EndNode();

		ImVec2 pos = imnodes::GetNodeGridSpacePos((int)m_id);
		m_xpos = pos.x;
		m_ypos = pos.y;
	}

	void mvNode::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) setNodePos((float)ToInt(item), (float)m_ypos);
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) setNodePos((float)m_xpos, (float)ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "draggable")) m_draggable = ToBool(item);

	}

	void mvNode::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "x_pos", ToPyInt(m_xpos));
		PyDict_SetItemString(dict, "y_pos", ToPyInt(m_ypos));	
		PyDict_SetItemString(dict, "draggable", ToPyBool(m_draggable));	
	}

}