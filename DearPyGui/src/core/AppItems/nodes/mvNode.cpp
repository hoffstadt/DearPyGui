#include "mvNode.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImNodesThemeScope.h"
#include "mvFontScope.h"

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
		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "draggable", "Allow node to be draggable.", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "x_pos", "x position the node will start at", "100"},
			{mvPythonDataType::Integer, "y_pos", "y position the node will start at", "100"},
		}, "Adds a node to a node editor.",
		"None", "Containers") });
	}

	mvNode::mvNode(const std::string& name)
		: mvAppItem(name)
	{
		m_description.container = true;
		m_core_config.label = FindRenderedTextEnd(m_core_config.name.c_str());
		m_label = m_core_config.label;
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

		for (auto item : m_children1)
		{
			// skip item if it's not shown
			if (!item->m_core_config.show)
				continue;

			// set item width
			if (item->m_core_config.width != 0)
				ImGui::SetNextItemWidth((float)item->m_core_config.width);

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

	PyObject* mvNode::add_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		int show = true;
		const char* label = "";
		int draggable = true;
		const char* parent = "";
		const char* before = "";
		int xpos = 100;
		int ypos = 100;

		if (!(mvApp::GetApp()->getParsers())["add_node"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &label, &draggable, &parent, &before, &xpos, &ypos))
			return ToPyBool(false);

		auto item = CreateRef<mvNode>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		auto topParent = mvApp::GetApp()->getItemRegistry().topParent();
		if (topParent)
		{
			if (topParent->getType() != mvAppItemType::mvNodeEditor)
			{
				ThrowPythonException("Parent on parent stack must be a node editor.");
				return ToPyBool(false);
			}
		}

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return ToPyString(name);

	}
}