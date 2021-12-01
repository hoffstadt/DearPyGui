#include "mvTabBar.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvTab.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTabBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		args.push_back({ mvPyDataType::Bool, "reorderable", mvArgType::KEYWORD_ARG, "False", "Allows for the user to change the order of the tabs." });

		mvPythonParserSetup setup;
		setup.about = "Adds a tab bar.";
		setup.category = { "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvTabBar::mvTabBar(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvTabBar::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvTabBar*>(item);
		_flags = titem->_flags;
	}

	mvUUID mvTabBar::getSpecificValue()
	{
		return _uiValue;
	}

	void mvTabBar::setValue(mvUUID value)
	{
		_uiValue = value;
	}

	PyObject* mvTabBar::getPyValue()
	{
		return ToPyUUID(*_value);
	}

	void mvTabBar::setPyValue(PyObject* value)
	{
		*_value = ToUUID(value);
	}

	void mvTabBar::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<mvUUID>*>(item->getValue());
	}

	void mvTabBar::draw(ImDrawList* drawlist, float x, float y)
	{

        if(!_show)
            return;
        
		ScopedID id(_uuid);

		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(_internalLabel.c_str(), _flags))
		{

            _state.lastFrameUpdate = GContext->frame;
            _state.visible = true; // BeginTabBar(...) only reaches this if visible
            
			for (auto& item : _children[1])
			{

				if (*_value == item->_uuid && _lastValue != *_value)
					static_cast<mvTab*>(item.get())->addFlag(ImGuiTabItemFlags_SetSelected);

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				if (*_value == item->_uuid)
					static_cast<mvTab*>(item.get())->removeFlag(ImGuiTabItemFlags_SetSelected);
			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();

		*_value = _uiValue;
		_lastValue = *_value;
	}

	void mvTabBar::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("reorderable", ImGuiTabBarFlags_Reorderable, _flags);

	}

	void mvTabBar::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		// window flags
		checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, _flags);
	}

}
