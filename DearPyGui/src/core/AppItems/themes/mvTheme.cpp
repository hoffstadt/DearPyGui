#include "mvTheme.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvThemeComponent.h"

namespace Marvel {

	void mvTheme::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Themes", "Containers" }, true);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID)
			);
			parser.finalize();
			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Themes" });
			parser.addArg<mvPyDataType::UUID>("theme");
			parser.finalize();
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
				if (_specificEnabled == comp->isEnabled())
				{
					child->draw(drawlist, x, y);
				}
				
			}
			if(comp->_specificType != (int)mvAppItemType::All)
			{
				if (_specificEnabled == comp->isEnabled())
				{
					comp->_oldComponent = *comp->_specificComponentPtr;
					*comp->_specificComponentPtr = child;
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
				if (_specificEnabled == comp->isEnabled())
				{
					child->customAction(data);
				}
			}
			if (comp->_specificType != (int)mvAppItemType::All)
			{
				if (_specificEnabled == comp->isEnabled())
				{
					*comp->_specificComponentPtr = comp->_oldComponent;
				}
			}
		}
	}

	void mvTheme::alternativeCustomAction(void* data)
	{
		if (!_default_theme)
		{
			_triggerAlternativeAction = false;
			return;
		}

		for (auto& childset : _children)
		{
			for (auto& child : childset)
				child->alternativeCustomAction(data);
		}
		_triggerAlternativeAction = false;
	}

	PyObject* mvTheme::bind_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["bind_theme"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		if (item == 0)
		{
			mvApp::GetApp()->resetTheme();
			return GetPyNone();
		}

		auto aplot = mvApp::GetApp()->getItemRegistry().getItem(item);
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

		graph->_default_theme = true;
		graph->_triggerAlternativeAction = true;

		return GetPyNone();
	}
}
