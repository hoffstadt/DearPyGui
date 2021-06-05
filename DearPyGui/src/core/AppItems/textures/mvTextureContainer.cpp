#include "mvTextureContainer.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"
#include "mvStaticTexture.h"
#include "mvDynamicTexture.h"

namespace Marvel {

	void mvTextureContainer::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTextureContainer::mvTextureContainer(const std::string& name)
		:
		mvAppItem(name)
	{
		m_show = false;
	}

	void mvTextureContainer::draw(ImDrawList* drawlist, float x, float y)
	{

		for (auto& item : m_children[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		if (m_show)
			show_debugger();

	}

	bool mvTextureContainer::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStaticTexture) return true;
		if (type == mvAppItemType::mvDynamicTexture) return true;

		mvThrowPythonError(1000, "Drawing children must be draw commands only.");
		MV_ITEM_REGISTRY_ERROR("Drawing children must be draw commands only.");
		assert(false);

		return false;
	}

	void mvTextureContainer::onChildRemoved(mvRef<mvAppItem> item)
	{
		m_selection = -1;
	}

	void mvTextureContainer::onChildrenRemoved()
	{
		m_selection = -1;
	}

	void mvTextureContainer::show_debugger()
	{
		ImGui::PushID(this);

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(m_label.c_str(), &m_show))
		{

			ImGui::Text("Textures");

			ImGui::BeginChild("##TextureStorageChild", ImVec2(400, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			int index = 0;
			for (auto& texture : m_children[1])
			{
				bool status = false;
				void* textureRaw = nullptr;
				if(texture->getType() == mvAppItemType::mvStaticTexture)
					textureRaw = static_cast<mvStaticTexture*>(texture.get())->getRawTexture();
				else
					textureRaw = static_cast<mvDynamicTexture*>(texture.get())->getRawTexture();

				ImGui::Image(textureRaw, ImVec2(25, 25));
				ImGui::SameLine();
				if (ImGui::Selectable(texture->m_name.c_str(), &status))
					m_selection = index;

				++index;
			}
			ImGui::EndChild();

			if (m_selection != -1)
			{

				ImGui::SameLine();
				ImGui::BeginGroup();

				ImGui::BeginGroup();
				ImGui::Text("Width: %d", m_children[1][m_selection]->getWidth());
				ImGui::Text("Height: %d", m_children[1][m_selection]->getHeight());
				ImGui::Text("Type: %s", m_children[1][m_selection]->getType() == mvAppItemType::mvStaticTexture ? "static" : "dynamic");
				ImGui::EndGroup();

				ImGui::SameLine();

				void* textureRaw = nullptr;
				if (m_children[1][m_selection]->getType() == mvAppItemType::mvStaticTexture)
					textureRaw = static_cast<mvStaticTexture*>(m_children[1][m_selection].get())->getRawTexture();
				else
					textureRaw = static_cast<mvDynamicTexture*>(m_children[1][m_selection].get())->getRawTexture();

				ImGui::Image(textureRaw, ImVec2((float)m_children[1][m_selection]->getWidth(), (float)m_children[1][m_selection]->getHeight()));

				ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				if (ImPlot::BeginPlot("##texture plot", 0, 0, ImVec2(-1, -1),
					ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_Equal))
				{
					ImPlot::PlotImage(m_children[1][m_selection]->m_name.c_str(), textureRaw, ImPlotPoint(0.0, 0.0),
						ImPlotPoint(m_children[1][m_selection]->getWidth(), m_children[1][m_selection]->getHeight()));
					ImPlot::EndPlot();
				}
				ImPlot::PopStyleColor();


				ImGui::EndGroup();
			}


		}

		ImGui::End();

		ImGui::PopID();
	}

}