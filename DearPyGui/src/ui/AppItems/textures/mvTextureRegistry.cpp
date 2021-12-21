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
		_show = false;
	}

	void mvTextureRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		for (auto& item : _children[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		if (_show)
			show_debugger();

	}

	void mvTextureRegistry::onChildRemoved(mvRef<mvAppItem> item)
	{
		_selection = -1;
	}

	void mvTextureRegistry::onChildrenRemoved()
	{
		_selection = -1;
	}

	void mvTextureRegistry::show_debugger()
	{
		ImGui::PushID(this);

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(_internalLabel.c_str(), &_show))
		{

			ImGui::Text("Textures");

			ImGui::BeginChild("##TextureStorageChild", ImVec2(400, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			int index = 0;
			for (auto& texture : _children[1])
			{
				bool status = false;
				void* textureRaw = nullptr;
				if(texture->_type == mvAppItemType::mvStaticTexture)
					textureRaw = static_cast<mvStaticTexture*>(texture.get())->getRawTexture();
				else
					textureRaw = static_cast<mvDynamicTexture*>(texture.get())->getRawTexture();

				ImGui::Image(textureRaw, ImVec2(25, 25));
				ImGui::SameLine();
				if (ImGui::Selectable(texture->_internalLabel.c_str(), &status))
					_selection = index;

				++index;
			}
			ImGui::EndChild();

			if (_selection != -1)
			{

				ImGui::SameLine();
				ImGui::BeginGroup();

				ImGui::BeginGroup();
				ImGui::Text("Width: %d", _children[1][_selection]->_width);
				ImGui::Text("Height: %d", _children[1][_selection]->_height);
				ImGui::Text("Type: %s", _children[1][_selection]->_type == mvAppItemType::mvStaticTexture ? "static" : "dynamic");
				ImGui::EndGroup();

				ImGui::SameLine();

				void* textureRaw = nullptr;
				if (_children[1][_selection]->_type == mvAppItemType::mvStaticTexture)
					textureRaw = static_cast<mvStaticTexture*>(_children[1][_selection].get())->getRawTexture();
				else
					textureRaw = static_cast<mvDynamicTexture*>(_children[1][_selection].get())->getRawTexture();

				ImGui::Image(textureRaw, ImVec2((float)_children[1][_selection]->_width, (float)_children[1][_selection]->_height));

				ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				if (ImPlot::BeginPlot("##texture plot", 0, 0, ImVec2(-1, -1),
					ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_Equal))
				{
					ImPlot::PlotImage(_children[1][_selection]->_internalLabel.c_str(), textureRaw, ImPlotPoint(0.0, 0.0),
						ImPlotPoint(_children[1][_selection]->_width, _children[1][_selection]->_height));
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