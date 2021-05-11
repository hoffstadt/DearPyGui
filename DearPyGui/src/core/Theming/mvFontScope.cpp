#include "mvFontScope.h"
#include <imgui.h>
#include "mvAppItem.h"
#include "mvFontManager.h"
#include "mvToolManager.h"

namespace Marvel {

	mvFontScope::mvFontScope(mvAppItem* item)
	{

		// cache is good
		if (item->isThemeFontCacheValid())
		{
			ImFont* font = item->getCachedFont();
			if (font)
			{
				ImGui::PushFont(font);
				m_valid = true;
			}
		}

		// item has it's own font
		else if (item->getFont())
		{
			item->setThemeFontCacheValid();
		}

		// item hasn't found its found yet
		else if (!item->getFontName().empty())
		{
			ImFont* font = mvToolManager::GetFontManager().getFont(item->getFontName(), item->getFontSize());
			item->setFont(item->getFontName(), item->getFontSize(), font);

			if(font)
				item->setThemeFontCacheValid();
		}

		// item doesn't have its own font
		else
		{
			// search through ancestor tree for font
			ImFont* font = nullptr;
			mvAppItem* widget = item;
			while (!mvAppItem::DoesItemHaveFlag(widget, MV_ITEM_DESC_ROOT))
			{
				widget = widget->getParent();

				if (font = widget->getFont())
					break;
			}

			if (font == nullptr)
				font = mvToolManager::GetFontManager().getFont("", 0);

			if (font)
			{
				ImGui::PushFont(font);
				item->setFont("", 0, font);
				m_valid = true;
				item->setThemeFontCacheValid();
			}

			//assert(false && "no font found");
			
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