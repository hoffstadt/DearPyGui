#include "mvMenuBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvMenuBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a menu bar to a window. Must be followed by a call to end.", "None", "Containers") });
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

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}
			ImGui::EndMenuBar();
		}
	}

}