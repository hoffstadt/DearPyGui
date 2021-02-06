#include "mvThemeManager.h"
#include "imgui.h"

namespace Marvel {

	void mvThemeManager::decodeType(long encoded_constant, mvAppItemType* type)
	{
		*type = (mvAppItemType)(encoded_constant / 1000);
	}
	void mvThemeManager::decodelibID(long encoded_constant, int* libID)
	{
		*libID = (int)((encoded_constant / 10) % 100);
	}


	mvThemeManager::mvThemeManager()
	{
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
		return event.handled;
	};

	bool mvThemeManager::add_color(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		decodeType(mvThemeConstant, &type);
		mvColor color = GetEColor(event, "COLOR");
		const std::string& widget = GetEString(event, "WIDGET");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			mvApp::GetApp()->getColors()[type][mvThemeConstant] = color;
			return true;
		}

		//check widget can take color and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
			item->getColors()[type][mvThemeConstant] = color;
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					ThrowPythonException("Item does not except this theme constant.");
				});
		}
		return true;
	}

	bool mvThemeManager::add_style(mvEvent& event)
	{
		static mvAppItemType type;
		long mvThemeConstant = GetELong(event, "ID");
		decodeType(mvThemeConstant, &type);
		float style = GetEFloat(event, "STYLE");
		const std::string& widget = GetEString(event, "WIDGET");

		//fills out the app's root theme if no item was given
		if (widget.empty())
		{
			mvApp::GetApp()->getStyles()[type][mvThemeConstant] = style;
			return true;
		}

		//check widget can take style and apply
		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(widget);
		if (item->getDescription().container || item->getType() == type)
		{
			item->getStyles()[type][mvThemeConstant] = style;
		}
		else
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					ThrowPythonException("Item does not except this theme constant.");
				});
		}
		return true;
	}

	mvImGuiThemeScope::mvImGuiThemeScope(mvAppItem* item)
	{

		const std::vector<std::pair<std::string, long>>& color_constants = item->getColorConstants();

		mvThemeColors colors;
		std::unordered_map<long, bool> colors_found;
		for (const auto& color_pair : color_constants)
			colors_found[color_pair.second] = false;

		// check local colors first
		if (item->getColors().find(item->getType()) != item->getColors().end())
		{
			for (const auto& color : item->getColors()[item->getType()])
			{
				colors_found[color.first] = true;
				colors[color.first] = color.second;
			}
		}

		// search through ancestor tree for unfound colors
		mvAppItem* widget = item;
		while (!widget->getDescription().root)
		{
			widget = widget->getParent();

			if (widget->getColors().find(item->getType()) != widget->getColors().end())
			{
				for (const auto& color : widget->getColors()[item->getType()])
				{
					// only apply if it wasn't found yet
					if(!colors_found[color.first])
						colors[color.first] = color.second;

					colors_found[color.first] = true;
				}
			}
		}

		for (auto& color : mvApp::GetApp()->getColors()[item->getType()])
		{
			// only apply if it wasn't found yet
			if (!colors_found[color.first])
				colors[color.first] = color.second;

			colors_found[color.first] = true;
		}

		libIDCount = colors.size();

		static int libID;
		for (const auto& color : colors) 
		{
			mvThemeManager::decodelibID(color.first, &libID);
			ImGui::PushStyleColor(libID, color.second.toVec4());
		}
	}

	mvImGuiThemeScope::~mvImGuiThemeScope()
	{
		cleanup();
	}

	void mvImGuiThemeScope::cleanup()
	{
		if (libIDCount > 0)
			ImGui::PopStyleColor(libIDCount);
		libIDCount = 0;
	}

}