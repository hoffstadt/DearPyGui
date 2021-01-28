#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvChild : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child, "add_child")

		mvChild(const std::string& name);

		void draw()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
		void addFlag           (ImGuiWindowFlags flag);
		void removeFlag        (ImGuiWindowFlags flag);

	private:

		bool             m_border     = true;
		bool             m_autosize_x = false;
		bool             m_autosize_y = false;
		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;

	};

}