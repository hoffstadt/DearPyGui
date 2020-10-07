#pragma once

#include "mvStandardWindow.h"

namespace Marvel {

	class mvFileDialog : public mvStandardWindow
	{

	public:

		mvFileDialog() : mvStandardWindow("FileDialog") {}

		bool prerender(bool& show) override { return true; }
		void setCallback(PyObject* callback) { m_callback = callback; }
		void render(bool& show) override;

	private:

		PyObject*   m_callback;
	};

}