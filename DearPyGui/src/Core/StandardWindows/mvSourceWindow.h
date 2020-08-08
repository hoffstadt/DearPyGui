#pragma once

#include "Core/mvTextEditor.h"
#include "mvStandardWindow.h"

namespace Marvel {

	class mvSourceWindow : public mvStandardWindow
	{

	public:

		void setFile(const std::string& file);

		void render(bool& show) override;

	private:

		mvTextEditor  m_editor;
		std::string  m_file;

	};

}