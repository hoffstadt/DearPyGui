#pragma once
#include "core/AppItems/mvTypeBases.h"
#include "mvAppItem.h"
#include "mvApp.h"
#include <ImGuiFileDialog.h>
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	class mvFileDialog : public mvBaseWindowAppitem
	{
		MV_APPITEM_TYPE(mvAppItemType::FileDialog, "no_command_set")

	public:

		mvFileDialog();

		void draw       () override;
		bool prerender2 ();
		void setCallback(PyObject* callback);

	private:

		PyObject*   m_callback;
	};

}