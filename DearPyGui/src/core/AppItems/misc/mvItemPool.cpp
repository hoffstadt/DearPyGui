#include "mvItemPool.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvAppItemCommons.h"

namespace Marvel {

	void mvItemPool::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers"}, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);
		parser.finalize();
		parsers->insert({ s_command, parser });
	}

	void mvItemSet::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);
		parser.addArg<mvPyDataType::Integer>("type");
		parser.addArg<mvPyDataType::Integer>("count");
		parser.finalize();
		parsers->insert({ s_command, parser });
	}

	mvItemSet::mvItemSet(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	mvItemPool::mvItemPool(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvItemSet::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_itemType = ToInt(item);
				break;

			case 1:
				_itemCount = ToInt(item);
				break;

			default:
				break;
			}
		}

		_availableItems.reserve(_itemCount);
		_returnedItems.reserve(_itemCount);
		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {
				using item_type = typename mvItemTypeMap<i>::type;
				mvAppItemType ait = mvItemTypeReverseMap<item_type>::type;
				if (_itemType == (int)ait)
				{
					for (int i = 0; i < _itemCount; i++)
					{
						_availableItems.push_back(CreateRef<item_type>(0));
						_availableItems.back()->setPoolInfo(_parent, _uuid);
					}
					return;
				}
			});
	}

	void mvItemSet::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		//PyDict_SetItemString(dict, "p1", ToPyPair(_p1.x, _p1.y));
		//PyDict_SetItemString(dict, "p2", ToPyPair(_p2.x, _p2.y));
		//PyDict_SetItemString(dict, "color", ToPyColor(_color));
		//PyDict_SetItemString(dict, "thickness", ToPyFloat(_thickness));
		//PyDict_SetItemString(dict, "size", ToPyFloat(_size));
	}

	mvRef<mvAppItem> mvItemSet::getItem()
	{
		return _availableItems[_currentAvailableIndex++];
	}

	void mvItemSet::returnItem(mvRef<mvAppItem> item)
	{
		_returnedItems[_currentReturnIndex] = item;
		_currentReturnIndex++;
	}

	mvRef<mvAppItem> mvItemPool::getItem(mvAppItemType itemType)
	{
		for (auto& child : _children[1])
		{
			if (static_cast<mvItemSet*>(child.get())->_itemType == (int)itemType)
			{
				auto item = static_cast<mvItemSet*>(child.get())->getItem();
				if (item)
					return item;
			}
		}

		return nullptr;
	}

	void mvItemPool::returnItem(mvRef<mvAppItem> item)
	{
		auto [pool, itemset] = item->getPoolInfo();

		for (auto& child : _children[1])
		{
			if (child->getUUID() == itemset)
			{
				static_cast<mvItemSet*>(child.get())->returnItem(item);
				break;
			}
		}
	}
}
