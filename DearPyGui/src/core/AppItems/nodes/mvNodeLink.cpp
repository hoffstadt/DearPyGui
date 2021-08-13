#include "mvNodeLink.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvNodeAttribute.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvNodeLink::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a node link between nodes.", { "Node Editor", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::UUID>("attr_1");
		parser.addArg<mvPyDataType::UUID>("attr_2");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNodeLink::mvNodeLink(mvUUID uuid)
		: mvAppItem(uuid)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		_id0 = (int)reduced_address;
	}

	void mvNodeLink::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				mvUUID node1 = mvAppItem::GetIDFromPyObject(item);
				mvAppItem* node = mvApp::GetApp()->getItemRegistry().getItem(node1);
				if (node->getType() == mvAppItemType::mvNodeAttribute)
				{
					_id1uuid = node1;
					_id1 = static_cast<const mvNodeAttribute*>(node)->getId();
				}
				else
				{
					mvThrowPythonError(mvErrorCode::mvIncompatibleType, s_command,
						"Incompatible type. Expected types include: mvNode", node);
					MV_ITEM_REGISTRY_ERROR("Nodes must be nodes. duh");
					assert(false);
					return;
				}
				break;
			}

			case 1:
			{
				mvUUID node2 = mvAppItem::GetIDFromPyObject(item);
				mvAppItem* node = mvApp::GetApp()->getItemRegistry().getItem(node2);
				if (node->getType() == mvAppItemType::mvNodeAttribute)
				{
					_id2uuid = node2;
					_id2 = static_cast<const mvNodeAttribute*>(node)->getId();
				}
				else
				{
					mvThrowPythonError(mvErrorCode::mvIncompatibleType, s_command,
						"Incompatible type. Expected types include: mvNode", node);
					MV_ITEM_REGISTRY_ERROR("Nodes must be nodes. duh");
					assert(false);
					return;
				}
				break;
			}

			default:
				break;
			}
		}
	}

	void mvNodeLink::customAction()
	{
		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------
		//_state._hovered = imnodes::IsLinkHovered(&_id);
		_state._visible = ImGui::IsItemVisible();
		//_state._active = imnodes::IsLinkStarted(&_id);
		//_state._deactivated = imnodes::IsLinkDropped(&_id);

		// pop class themes
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->customAction();

		// pop item themes
		if (_theme)
			static_cast<mvTheme*>(_theme.get())->customAction();

		// event handlers
		for (auto& item : _children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
	}

	void mvNodeLink::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		// show/hide
		if (!_show)
			return;

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		// handle class theming
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		// handle item theming
		if (_theme)
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		ScopedID id(_uuid);

		imnodes::Link(_id0, _id1, _id2);

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------
		_state._lastFrameUpdate = mvApp::s_frame;
		// check custom action

	}

	void mvNodeLink::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "attr_1", ToPyUUID(_id1uuid));
		PyDict_SetItemString(dict, "attr_2", ToPyUUID(_id2uuid));
	}

}