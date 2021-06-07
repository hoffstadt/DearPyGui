#include "mvNodeLink.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvLog.h"
#include "mvNodeAttribute.h"
#include "mvItemRegistry.h"
#include "mvImNodesThemeScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvNodeLink::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Node Editor", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::String>("node_1");
		parser.addArg<mvPyDataType::String>("node_2");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvNodeLink::mvNodeLink(const std::string& name)
		: mvAppItem(name)
	{
		int64_t address = (int64_t)this;
		int64_t reduced_address = address % 2147483648;
		m_id = (int)reduced_address;
	}

	bool mvNodeLink::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvNodeEditor)
			return true;

		mvThrowPythonError(1000, "Node link parent must be node editor.");
		MV_ITEM_REGISTRY_ERROR("Node link parent must be node editor.");
		assert(false);
		return false;
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
				std::string node1 = ToString(item);
				mvAppItem* node = mvApp::GetApp()->getItemRegistry().getItem(node1);
				if (node->getType() == mvAppItemType::mvNodeAttribute)
				{
					m_id1 = static_cast<const mvNodeAttribute*>(node)->getId();
				}
				else
				{
					mvThrowPythonError(1000, "Nodes must be nodes. duh");
					MV_ITEM_REGISTRY_ERROR("Nodes must be nodes. duh");
					assert(false);
					return;
				}
				break;
			}

			case 1:
			{
				std::string node2 = ToString(item);
				mvAppItem* node = mvApp::GetApp()->getItemRegistry().getItem(node2);
				if (node->getType() == mvAppItemType::mvNodeAttribute)
				{
					m_id2 = static_cast<const mvNodeAttribute*>(node)->getId();
				}
				else
				{
					mvThrowPythonError(1000, "Nodes must be nodes. duh");
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

	void mvNodeLink::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImNodesThemeScope scope(this);
		
		imnodes::Link(m_id, m_id1, m_id2);
	}

}