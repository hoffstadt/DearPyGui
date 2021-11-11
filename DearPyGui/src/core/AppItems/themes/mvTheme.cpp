#include "mvTheme.h"
#include "mvContext.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvThemeComponent.h"

namespace Marvel {

	void mvTheme::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			std::vector<mvPythonDataElement> args;

			AddCommonArgs(args,(CommonParserArgs)(
				MV_PARSER_ARG_ID)
			);

			args.push_back({ mvPyDataType::Bool, "default_theme", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });

			mvPythonParserSetup setup;
			setup.about = "Adds a theme.";
			setup.category = { "Themes", "Containers" };
			setup.returnType = mvPyDataType::UUID;
			setup.createContextManager = true;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ s_command, parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "theme" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global theme.";
			setup.category = { "Themes"};

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "bind_theme", parser });
		}

	}

	mvTheme::mvTheme(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = false;
	}

	void mvTheme::draw(ImDrawList* drawlist, float x, float y)
	{
		for (auto& child : _children[1])
		{
			auto comp = static_cast<mvThemeComponent*>(child.get());
			if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					child->draw(drawlist, x, y);
				}
				
			}
			if(comp->_specificType != _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					comp->_oldComponent = *comp->_specificComponentPtr;
					*comp->_specificComponentPtr = child;
				}
				else
				{
					comp->_oldComponent = *comp->_specificDisabledComponentPtr;
					*comp->_specificDisabledComponentPtr = child;
				}
			}
		}
	}

	void mvTheme::customAction(void* data)
	{

		for (auto& child : _children[1])
		{
			auto comp = static_cast<mvThemeComponent*>(child.get());
			if (comp->_specificType == (int)mvAppItemType::All || comp->_specificType == _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					child->customAction(data);
				}
			}
			if (comp->_specificType != _specificType)
			{
				if (_specificEnabled == comp->_specificEnabled)
				{
					*comp->_specificComponentPtr = comp->_oldComponent;
				}
				else
				{
					*comp->_specificDisabledComponentPtr = comp->_oldComponent;
				}
			}
		}
	}

	PyObject* mvTheme::bind_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["bind_theme"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = GetIDFromPyObject(itemraw);

		if (item == 0)
		{
			GContext->resetTheme = true;
			ResetTheme((*GContext->itemRegistry));
			return GetPyNone();
		}

		auto aplot = GetItem((*GContext->itemRegistry), item);
		if (aplot == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_theme",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (aplot->getType() != mvAppItemType::mvTheme)
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "bind_theme",
				"Incompatible type. Expected types include: mvTheme", aplot);
			return GetPyNone();
		}

		mvTheme* graph = static_cast<mvTheme*>(aplot);

		ResetTheme(*GContext->itemRegistry);

		graph->_show = true;

		return GetPyNone();
	}
}
