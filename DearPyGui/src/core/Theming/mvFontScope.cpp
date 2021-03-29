#include "mvFontScope.h"
#include <imgui.h>
#include "mvAppItem.h"


namespace Marvel {

	mvFontScope::mvFontScope(mvAppItem* item)
	{
		ImFont* font = item->getCachedFont();

		if (!item->isThemeFontCacheValid())
		{
			// search through ancestor tree for font
			mvAppItem* widget = item;
			while (!widget->getDescription().root)
			{
				widget = widget->getParent();

				if (widget->getCachedFont())
				{
					ImGui::PushFont(widget->getCachedFont());
					break;
				}
			}

			item->setThemeFontCacheValid();
		}

		if (font)
			m_valid = true;

	}

	mvFontScope::~mvFontScope()
	{
		cleanup();
	}

	void mvFontScope::cleanup()
	{
		if (m_valid)
			ImGui::PopFont();
	}

}