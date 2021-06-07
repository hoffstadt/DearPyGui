#include "mvThemeManager.h"
#include "imgui.h"
#include "mvAppItemCommons.h"
#include "mvCore.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	std::vector<mvThemeColorGroup::mvThemeColor*> mvThemeManager::s_acolors;
	std::vector<mvThemeColorGroup::mvThemeColor*> mvThemeManager::s_adcolors;
	std::vector<mvThemeStyleGroup::mvThemeStyle*> mvThemeManager::s_astyles;
	std::vector<mvThemeColorGroup::mvThemeColor>  mvThemeManager::s_colors;
	std::vector<mvThemeColorGroup::mvThemeColor>  mvThemeManager::s_disabled_colors;
	std::vector<mvThemeStyleGroup::mvThemeStyle>  mvThemeManager::s_styles;

	void mvThemeManager::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Integer>("constant");
			parser.addArg<mvPyDataType::FloatList>("color");
			parser.addArg<mvPyDataType::String>("item", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "set_theme_color", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Integer>("constant");
			parser.addArg<mvPyDataType::FloatList>("color");
			parser.addArg<mvPyDataType::String>("item", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "set_theme_color_disabled", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.addArg<mvPyDataType::Integer>("constant");
			parser.addArg<mvPyDataType::Float>("style");
			parser.addArg<mvPyDataType::String>("item", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "set_theme_style", parser });
		}

	}

	void mvThemeManager::InValidateColorTheme()
	{
		auto& frontWindows = mvApp::GetApp()->getItemRegistry().getRoots();

		for (auto& window : frontWindows)
			window->inValidateThemeColorCache();
	}

	void mvThemeManager::InValidateDisabledColorTheme()
	{
		auto& frontWindows = mvApp::GetApp()->getItemRegistry().getRoots();

		for (auto& window : frontWindows)
			window->inValidateThemeDisabledColorCache();
	}

	void mvThemeManager::InValidateStyleTheme()
	{
		auto& frontWindows = mvApp::GetApp()->getItemRegistry().getRoots();

		for (auto& window : frontWindows)
			window->inValidateThemeStyleCache();
	}

	mvThemeManager::mvThemeManager()
	{
		mvEventBus::Subscribe(this, SID("disabled_color_change"), mvEVT_CATEGORY_THEMES);
		mvEventBus::Subscribe(this, SID("color_change"), mvEVT_CATEGORY_THEMES);
		mvEventBus::Subscribe(this, SID("style_change"), mvEVT_CATEGORY_THEMES);
	}

	mvThemeManager::~mvThemeManager()
	{
		mvEventBus::UnSubscribe(this);
	}
 
	bool mvThemeManager::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);
		dispatcher.dispatch(BIND_EVENT_METH(mvThemeManager::add_style), SID("style_change"));
		dispatcher.dispatch(BIND_EVENT_METH(mvThemeManager::add_color), SID("color_change"));
		dispatcher.dispatch(BIND_EVENT_METH(mvThemeManager::add_disabled_color), SID("disabled_color_change"));
		return event.handled;
	};

	bool mvThemeManager::add_color(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		DecodeType(mvThemeConstant, &type);
		mvColor color = GetEColor(event, "COLOR");
		const std::string& widget = GetEString(event, "WIDGET");
		bool enabled = GetEBool(event, "ENABLED");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			for (auto& existingColor : s_colors)
			{
				if (existingColor.constant == mvThemeConstant)
				{
					existingColor.color = color;
					InValidateColorTheme();
					return true;
				}
			}

			assert(false && "color not found");
		}

		//check widget can take color and apply
		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item)
		{
			if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER) || item->getType() == type)
			{
				item->getColorGroup().addColor(GetNameFromConstant(mvThemeConstant), mvThemeConstant, color);
				item->inValidateThemeColorCache();
			}
			else
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						mvThrowPythonError(1000, "Item type does not except this color constant.");
					});
			}
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvThrowPythonError(1000, "Item can not be found");
				});
		}
		return true;
	}

	bool mvThemeManager::add_disabled_color(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		DecodeType(mvThemeConstant, &type);
		mvColor color = GetEColor(event, "COLOR");
		const std::string& widget = GetEString(event, "WIDGET");
		bool enabled = GetEBool(event, "ENABLED");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			for (auto& existingColor : s_disabled_colors)
			{
				if (existingColor.constant == mvThemeConstant)
				{
					existingColor.color = color;
					InValidateDisabledColorTheme();
					return true;
				}
			}

			assert(false && "color not found");
		}

		//check widget can take color and apply
		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item)
		{
			if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER) || item->getType() == type)
			{
				item->getDisabledColorGroup().addColor(GetNameFromConstant(mvThemeConstant), mvThemeConstant, color);
				item->inValidateThemeDisabledColorCache();
			}
			else
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						mvThrowPythonError(1000, "Item type does not except this color constant.");
					});
			}
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvThrowPythonError(1000, "Item can not be found");
				});
		}
		return true;
	}

	bool mvThemeManager::add_style(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		DecodeType(mvThemeConstant, &type);
		float style = GetEFloat(event, "STYLE");
		const std::string& widget = GetEString(event, "WIDGET");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			for (auto& existingStyle : s_styles)
			{
				if (existingStyle.constant == mvThemeConstant)
				{
					existingStyle.value1 = style;
					existingStyle.value2 = style;
					InValidateStyleTheme();
					return true;
				}
			}

			assert(false && "style not found");
		}

		//check widget can take style and apply
		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item)
		{
			if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER) || item->getType() == type)
			{
				item->getStyleGroup().addStyle(GetNameFromConstant(mvThemeConstant), mvThemeConstant, style, style);
				item->inValidateThemeStyleCache();
			}
			else
			{
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						mvThrowPythonError(1000, "Item type does not except this style constant.");
					});
			}
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					mvThrowPythonError(1000, "Item can not be found");
				});
		}
		return true;
	}

	PyObject* mvThemeManager::set_theme_color(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		long constant;
		PyObject* color;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_theme_color"].parse(args, kwargs, __FUNCTION__, &constant, &color, &item))
			return GetPyNone();

		Py_XINCREF(color);
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvEventBus::Publish
		(
			mvEVT_CATEGORY_THEMES,
			SID("color_change"),
			{
				CreateEventArgument("WIDGET", std::string(item)),
				CreateEventArgument("ID", constant),
				CreateEventArgument("COLOR", ToColor(color)),
				CreateEventArgument("ENABLED", true)
			}
		);

		Py_XDECREF(color);


		return GetPyNone();
	}

	PyObject* mvThemeManager::set_theme_color_disabled(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		long constant;
		PyObject* color;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_theme_color_disabled"].parse(args, kwargs, __FUNCTION__, &constant, &color, &item))
			return GetPyNone();

		Py_XINCREF(color);
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvEventBus::Publish
				(
					mvEVT_CATEGORY_THEMES,
					SID("disabled_color_change"),
					{
						CreateEventArgument("WIDGET", std::string(item)),
						CreateEventArgument("ID", constant),
						CreateEventArgument("COLOR", ToColor(color)),
						CreateEventArgument("ENABLED", false)
					}
				);

				// to ensure the decrement happens on the python thread
				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						Py_XDECREF(color);
					});

			});

		return GetPyNone();
	}

	PyObject* mvThemeManager::set_theme_style(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		long constant;
		float style;
		const char* item = "";

		if (!(mvApp::GetApp()->getParsers())["set_theme_style"].parse(args, kwargs, __FUNCTION__, &constant, &style, &item))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getCallbackRegistry().submit([=]()
			{
				mvEventBus::Publish
				(
					mvEVT_CATEGORY_THEMES,
					SID("style_change"),
					{
						CreateEventArgument("WIDGET", std::string(item)),
						CreateEventArgument("ID", constant),
						CreateEventArgument("STYLE", style)
					}
				);
			});

		return GetPyNone();
	}

	std::vector<mvThemeColorGroup::mvThemeColor> mvThemeManager::GetColorsByType(mvAppItemType type, bool disabled)
	{
		std::vector<mvThemeColorGroup::mvThemeColor> colors;

		for (const auto& color : disabled ? s_disabled_colors : s_colors)
		{
			if ((mvAppItemType)mvThemeColorGroup::DecodeType(color.constant) == type)
				colors.push_back(color);
		}

		return colors;
	}

	std::vector<mvThemeStyleGroup::mvThemeStyle> mvThemeManager::GetStylesByType(mvAppItemType type)
	{
		std::vector<mvThemeStyleGroup::mvThemeStyle> styles;

		for (const auto& style : s_styles)
		{
			if ((mvAppItemType)mvThemeColorGroup::DecodeType(style.constant) == type)
				styles.push_back(style);
		}

		return styles;
	}

	const std::string& mvThemeManager::GetNameFromConstant(long constant)
	{
		static std::string empty;

		for (const auto& color : s_colors)
		{
			if (color.constant == constant)
				return color.name;
		}

		for (const auto& style : s_styles)
		{
			if (style.constant == constant)
				return style.name;
		}

		assert(false);
		return empty;
	}

}