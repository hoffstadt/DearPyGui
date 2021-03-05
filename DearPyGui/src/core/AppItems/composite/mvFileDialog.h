#pragma once
#include "mvTypeBases.h"
#include "mvApp.h"
#include <ImGuiFileDialog.h>

namespace Marvel {

#ifdef MV_CPP
#else
	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* open_file_dialog       (PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	class mvFileDialog : public mvBaseWindowAppitem
	{

		MV_APPITEM_TYPE(mvAppItemType::FileDialog, mvFileDialog, "no_command_set")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvFileDialog();

		void draw       () override;
		bool prerender2 ();
		void setCallback(mvCallable callback);

	private:

		mvCallable   m_callback2 = nullptr;
	};

}