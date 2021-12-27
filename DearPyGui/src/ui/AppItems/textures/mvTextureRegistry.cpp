#include "mvTextureRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"
#include "mvStaticTexture.h"
#include "mvDynamicTexture.h"

namespace Marvel {

	mvTextureRegistry::mvTextureRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		config.show = false;
	}

	void mvTextureRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		for (auto& item : childslots[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		if (config.show)
			show_debugger();

	}

	void mvTextureRegistry::show_debugger()
	{
		ImGui::PushID(this);

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(info.internalLabel.c_str(), &config.show))
		{

			ImGui::Text("Textures");

			ImGui::BeginChild("##TextureStorageChild", ImVec2(400, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			int index = 0;
			for (auto& texture : childslots[1])
			{
				bool status = false;
				void* textureRaw = nullptr;
				if(texture->type == mvAppItemType::mvStaticTexture)
					textureRaw = static_cast<mvStaticTexture*>(texture.get())->getRawTexture();
				else
					textureRaw = static_cast<mvDynamicTexture*>(texture.get())->getRawTexture();

				ImGui::Image(textureRaw, ImVec2(25, 25));
				ImGui::SameLine();
				if (ImGui::Selectable(texture->info.internalLabel.c_str(), &status))
					_selection = index;

				++index;
			}
			ImGui::EndChild();

			if (_selection != -1)
			{

				if (_selection < childslots[1].size())
				{

					ImGui::SameLine();
					ImGui::BeginGroup();

					ImGui::BeginGroup();
					ImGui::Text("Width: %d", childslots[1][_selection]->config.width);
					ImGui::Text("Height: %d", childslots[1][_selection]->config.height);
					ImGui::Text("Type: %s", childslots[1][_selection]->type == mvAppItemType::mvStaticTexture ? "static" : "dynamic");
					ImGui::EndGroup();

					ImGui::SameLine();

					void* textureRaw = nullptr;
					if (childslots[1][_selection]->type == mvAppItemType::mvStaticTexture)
						textureRaw = static_cast<mvStaticTexture*>(childslots[1][_selection].get())->getRawTexture();
					else
						textureRaw = static_cast<mvDynamicTexture*>(childslots[1][_selection].get())->getRawTexture();

					ImGui::Image(textureRaw, ImVec2((float)childslots[1][_selection]->config.width, (float)childslots[1][_selection]->config.height));

					ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					if (ImPlot::BeginPlot("##texture plot", 0, 0, ImVec2(-1, -1),
						ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_Equal))
					{
						ImPlot::PlotImage(childslots[1][_selection]->info.internalLabel.c_str(), textureRaw, ImPlotPoint(0.0, 0.0),
							ImPlotPoint(childslots[1][_selection]->config.width, childslots[1][_selection]->config.height));
						ImPlot::EndPlot();
					}
					ImPlot::PopStyleColor();


					ImGui::EndGroup();
				}
				else
					_selection = -1;
			}


		}

		ImGui::End();

		ImGui::PopID();
	}

}