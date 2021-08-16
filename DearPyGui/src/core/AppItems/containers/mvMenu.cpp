#include "mvMenu.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvMenu::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a menu to an existing menu bar. Must be followed by a call to end.", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_ENABLED)
		);
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenu::mvMenu(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvMenu::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		// create menu and see if its selected
		if (ImGui::BeginMenu(_internalLabel.c_str(), _enabled))
		{

			// set other menus's value false on same level
			for (auto& sibling : _parentPtr->getChildren(1))
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::mvMenu)
					*((mvMenu*)sibling.get())->_value = false;
			}

			// set current menu value true
			*_value = true;

			for (auto& item : _children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}

			registerWindowFocusing();

			ImGui::EndMenu();
		}

	}

	void mvMenu::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) _enabled = ToBool(item);

	}

	void mvMenu::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "enabled", mvPyObject(ToPyBool(_enabled)));
	}

	PyObject* mvMenu::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvMenu::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvMenu::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}

}