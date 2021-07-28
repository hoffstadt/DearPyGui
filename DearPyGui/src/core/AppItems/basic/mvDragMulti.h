#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloatMulti
//     * mvDragIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvDragFloatMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvDragFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvDragFloatMulti : public mvFloat4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragFloatMulti, add_drag_floatx)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDragFloatMulti(mvUUID uuid);

		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		float               _speed = 1.0f;
		float               _min = 0.0f;
		float               _max = 100.0f;
		std::string         _format = "%.3f";
		ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;
		int                 _size = 4;
	};

	//-----------------------------------------------------------------------------
	// mvDragIntMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvDragIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
	class mvDragIntMulti : public mvInt4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDragIntMulti, add_drag_intx)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDragIntMulti(mvUUID uuid);

		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		float               _speed = 1.0f;
		int                 _min = 0;
		int                 _max = 100;
		std::string         _format = "%d";
		ImGuiInputTextFlags _flags = ImGuiSliderFlags_None;
		ImGuiInputTextFlags _stor_flags = ImGuiSliderFlags_None;
		int                 _size = 4;
	};
}