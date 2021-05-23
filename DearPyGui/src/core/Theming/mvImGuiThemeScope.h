#pragma once

#include <unordered_map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "mvStyleWindow.h"
#include "mvThemeManager.h"

namespace Marvel {

	template<typename T>
	class mvImGuiThemeScope
	{

	public:

		mvImGuiThemeScope(T* item)
		{

			//-----------------------------------------------------------------------------
			// colors
			//-----------------------------------------------------------------------------
			
			// updates colors if cache is invalid and caches (disabled and regular)
			SearchAncestorsForColors(item);
			libIDCount = GetCacheColorCount(item);
			static ImGuiCol imColorID;
			if (item->isEnabled())
			{
				for (const auto& color : item->getColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					ImGui::PushStyleColor(imColorID, color.color.toVec4());
				}
			}
			else
			{
				for (const auto& color : item->getDisabledColorGroup().getCachedColors())
				{
					DecodelibID(color.constant, &imColorID);
					ImGui::PushStyleColor(imColorID, color.color.toVec4());
				}
			}

			//-----------------------------------------------------------------------------
			// styles
			//-----------------------------------------------------------------------------
			
			// updates styles if cache is invalid and caches
			SearchAncestorsForStyles(item);
			StyleIDCount = item->getStyleGroup().getCachedStyles().size() + item->getStyleGroup().getCachedStyles2().size();
			static ImGuiStyleVar imstyleID;
			for (const auto& style : item->getStyleGroup().getCachedStyles2())
			{
				DecodelibID(style.constant, &imstyleID);
				ImGui::PushStyleVar(imstyleID, { style.value1, style.value2 });
			}

			for (const auto& style : item->getStyleGroup().getCachedStyles())
			{
				DecodelibID(style.constant, &imstyleID);
				ImGui::PushStyleVar(imstyleID, style.value1);
			}
		}

		~mvImGuiThemeScope()
		{
			cleanup();
		}

		void cleanup()
		{
			if (libIDCount > 0)
				ImGui::PopStyleColor((int)libIDCount);
			libIDCount = 0;
			if (StyleIDCount > 0)
				ImGui::PopStyleVar((int)StyleIDCount);
			StyleIDCount = 0;
		}

	private:

		size_t libIDCount = 0;
		size_t StyleIDCount = 0;

	};
}