#pragma once
#include "mvTextEditor.h"
#include "mvTypeBases.h"

namespace Marvel {

	class mvSourceWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::SourceWindow, "add_source_window")

	public:

		mvSourceWindow(const std::string& name) : 
			mvBaseWindowAppitem(name)
		{
			m_windowflags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;
		}

		void setFile(const std::string& file);

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		mvTextEditor  m_editor;
		std::string  m_file;

	};

}