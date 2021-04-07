#pragma once
#include "mvTypeBases.h"
#include "mvApp.h"
#include <ImGuiFileDialog.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvFileDialog, MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvFileDialog : public mvBaseWindowAppitem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
		
		//MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFileDialog, open_file_dialog)
		static constexpr const long s_internal_type = (long)mvAppItemType::mvFileDialog;
		static constexpr const char* const s_internal_id = "mvAppItemType::mvFileDialog";
		static constexpr const char* const s_command = "open_file_dialog";
		mvAppItemType getType() const override { return mvAppItemType::mvFileDialog; }\
		int getTarget() const override { return mvItemTypeMap<(int)mvAppItemType::mvFileDialog>::target; }
		int getDescFlags() const override { return mvItemTypeMap<(int)mvAppItemType::mvFileDialog>::flags; }
		StorageValueTypes getValueType() const override { return mvItemTypeMap<(int)mvAppItemType::mvFileDialog>::value_type; }
		static PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyMethodDef GetMethodDefinition() { return { s_command, (PyCFunction)open_file_dialog, METH_VARARGS | METH_KEYWORDS, mvApp::GetApp()->getParsers()[s_command].getDocumentation() }; }

		MV_CREATE_EXTRA_COMMAND(select_directory_dialog);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(select_directory_dialog);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvFileDialog();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool prerender2 ();
		void setCallback(mvCallable callback);

	private:

		mvCallable   m_callback2 = nullptr;
	};

}