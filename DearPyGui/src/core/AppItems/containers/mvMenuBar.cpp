#include "mvMenuBar.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvMenuBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("before");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenuBar::mvMenuBar(const std::string& name)
			: mvBoolPtrBase(name)
		{

			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_height = 21;
		}

	void mvMenuBar::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::BeginMenuBar())
		{
			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				if (item->m_focusNextFrame)
				{
					ImGui::SetKeyboardFocusHere();
					item->m_focusNextFrame = false;
				}

				auto oldCursorPos = ImGui::GetCursorPos();
				if (item->m_dirtyPos)
					ImGui::SetCursorPos(item->getState().getItemPos());

				item->getState().setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				if (item->m_dirtyPos)
					ImGui::SetCursorPos(oldCursorPos);

				item->getState().update();
			}
			ImGui::EndMenuBar();
		}
	}

	bool mvMenuBar::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvChild) return true;

		mvThrowPythonError(1000, "Menubar parent must be a window or child.");
		MV_ITEM_REGISTRY_ERROR("Menubar parent must be a window or child.");
		assert(false);
		return false;
	}

}