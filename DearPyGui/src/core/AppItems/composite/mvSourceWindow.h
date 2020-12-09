#pragma once
#include "mvTextEditor.h"
#include "mvTypeBases.h"

namespace Marvel {

	class mvSourceWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::SourceWindow, "add_source_window")

	public:

		mvSourceWindow(const std::string& name) : 
			mvBaseWindowAppitem(name)
		{
			m_windowflags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;
			m_description.deleteAllowed = false;
		}

		void setFile(const std::string& file);

		void draw() override;

	private:

		mvTextEditor  m_editor;
		std::string  m_file;

	};

}