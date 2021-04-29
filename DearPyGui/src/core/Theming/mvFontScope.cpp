#include "mvFontScope.h"
#include <imgui.h>
#include "mvAppItem.h"
#include "mvFontManager.h"
#include "mvToolManager.h"

namespace Marvel {

	mvFontScope::mvFontScope(mvAppItem* item)
	{

		if (item->isThemeFontCacheValid())
		{
			ImFont* font = item->getCachedFont();
			if (font)
			{
				ImGui::PushFont(font);
				m_valid = true;
			}
		}
		else
		{
			// search through ancestor tree for font
			ImFont* font = nullptr;
			mvAppItem* widget = item;
			font = widget->getCachedFont();
			while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
			{
				widget = widget->getParent();

				if (font = widget->getCachedFont())
					break;
			}

			if (font == nullptr)
				font = mvToolManager::GetFontManager().getFont("", 0);

			if (font)
			{
				ImGui::PushFont(font);
				item->setFont(font);
				m_valid = true;
			}

			item->setThemeFontCacheValid();
		}

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