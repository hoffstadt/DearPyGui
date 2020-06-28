#pragma once

#include "Core/mvTextEditor.h"

namespace Marvel {

	class mvSourceWindow
	{

	public:

		static mvSourceWindow* GetWindow();

		void render (bool& show);
		void setFile(const std::string& file);

	private:

		mvSourceWindow() = default;

	private:

		static mvSourceWindow* s_instance;
		mvTextEditor           m_editor;
		std::string            m_file;

	};

}