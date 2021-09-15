#include "mvItemPool.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvAppItemCommons.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvItemPool::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds an item pool.";
		setup.category = { "Containers" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);
		parsers->insert({ s_command, parser });
	}

	void mvItemSet::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;
		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);
		args.push_back({ mvPyDataType::Integer, "type" });
		args.push_back({ mvPyDataType::Integer, "count" });

		mvPythonParserSetup setup;
		setup.about = "Adds an item set to an item pool.";
		setup.category = { "Containers" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);
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
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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
			if (child->_uuid == itemset)
			{
				static_cast<mvItemSet*>(child.get())->returnItem(item);
				break;
			}
		}
	}
}
