#include "mvFilterSet.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvFilterSet::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvFilterSet::mvFilterSet(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvFilterSet::getPyValue()
	{
		return ToPyString(std::string(m_imguiFilter.InputBuf));
	}

	void mvFilterSet::setPyValue(PyObject* value)
	{
		auto str_value = ToString(value);

		int i = 0;
		for (auto& character : str_value)
		{
			if (i > 254)
				break;

			m_imguiFilter.InputBuf[i] = character;

			++i;
		}
		m_imguiFilter.InputBuf[i] = 0;
		m_imguiFilter.Build();
	}

	void mvFilterSet::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvFontScope fscope(this);

		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);

		if (m_imguiFilter.IsActive())
		{
			for (auto& childset : m_children)
			{
				for (auto& child : childset)
				{
					if (!m_imguiFilter.PassFilter(child->getFilter().c_str()))
						continue;

					if (!child->preDraw())
						continue;
					child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
					child->postDraw();
				}
			}

		}
		else
		{

			for (auto& childset : m_children)
			{
				for (auto& child : childset)
				{
					if (!child->preDraw())
						continue;
					child->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
					child->postDraw();
				}
			}
		}

		if (m_width != 0)
			ImGui::PopItemWidth();
	}

}