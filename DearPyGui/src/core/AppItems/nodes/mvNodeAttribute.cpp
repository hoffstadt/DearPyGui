#include "mvNodeAttribute.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvCore.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvNodeEditor.h"
#include "mvImNodesThemeScope.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvNodeAttribute::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Bool>("output", mvArgType::KEYWORD_ARG, "False", "Set as output attribute");
		parser.addArg<mvPyDataType::Bool>("static", mvArgType::KEYWORD_ARG, "False", "Set as static attribute");

		parser.addArg<mvPyDataType::Integer>("shape", mvArgType::KEYWORD_ARG, "54010", "Pin shape");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNodeAttribute::mvNodeAttribute(const std::string& name)
		: mvAppItem(name)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		m_id = (int)reduced_address;
	}

	bool mvNodeAttribute::isParentCompatible(mvAppItemType type)
	{
		if(type == mvAppItemType::mvNode)
			return true;
		
		mvThrowPythonError(1000, "Node attribute parent must be node.");
		MV_ITEM_REGISTRY_ERROR("Node attribute parent must be node.");
		assert(false);
		return false;
	}

	void mvNodeAttribute::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImNodesThemeScope scope(this);
		mvFontScope fscope(this);

		if (m_static)
			imnodes::BeginStaticAttribute((int)m_id);
		else if(m_output)
			imnodes::BeginOutputAttribute((int)m_id, m_shape);
		else
			imnodes::BeginInputAttribute((int)m_id, m_shape);

		//we do this so that the children dont get the theme
		//scope.cleanup();

		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			if (item->m_focusNextFrame)
			{
				ImGui::SetKeyboardFocusHere();
				item->m_focusNextFrame = false;
			}

			if (item->m_dirtyPos)
			{
				ImGui::SetCursorPos(item->getState().getItemPos());
				item->m_dirtyPos = false;
			}
			item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

			item->draw(drawlist, x, y);

			item->getState().update();
		}

		if (m_static)
			imnodes::EndStaticAttribute();
		else if (m_output)
			imnodes::EndOutputAttribute();
		else
			imnodes::EndInputAttribute();
	}

	void mvNodeAttribute::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "output")) m_output = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "static")) m_static = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "shape"))
		{
			m_shape = (imnodes::PinShape)ToInt(item);
			DecodelibID(m_shape, (int*)&m_shape);
		}
	}

	void mvNodeAttribute::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "output", ToPyBool(m_output));
		PyDict_SetItemString(dict, "static", ToPyBool(m_static));
		PyDict_SetItemString(dict, "shape", ToPyInt(MV_ENCODE_CONSTANT((int)m_shape, 0)));
	}

}