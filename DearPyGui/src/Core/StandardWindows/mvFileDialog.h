#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvFileDialog : public mvStandardWindow
	{

	public:

		void setCallback(const char* callback) { m_callback = callback; }

		void render(bool& show) override;

	private:

		std::string m_callback;
		std::string m_filePath;
		std::string m_filePathName;
	};

}