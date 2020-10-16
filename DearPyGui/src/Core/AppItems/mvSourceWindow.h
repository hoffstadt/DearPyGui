#pragma once
#include "Core/mvTextEditor.h"
#include "Core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"

namespace Marvel {

	class mvSourceWindow : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::SourceWindow, "add_source_window")

	public:

		mvSourceWindow(const std::string& name) : 
			mvBaseWindowAppitem(name)
		{
			m_windowflags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;
		}

		bool canBeDeleted() const override { return false; }

		void setFile(const std::string& file);

		void draw() override;

	private:

		mvTextEditor  m_editor;
		std::string  m_file;

	};

}