#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"
#include <string>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputIntMulti
//     * mvInputFloatMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {
	  
	//-----------------------------------------------------------------------------
	// mvInputIntMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvInputIntMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int4, 1);
	class mvInputIntMulti : public mvInt4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputIntMulti, add_input_intx)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvInputIntMulti(mvUUID uuid);
		
		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int                 _min = 0;
		int                 _max = 100;
		bool                _min_clamped = false;
		bool                _max_clamped = false;
		ImGuiInputTextFlags _flags = 0;
		ImGuiInputTextFlags _stor_flags = 0;
		std::array<int, 4>  _last_value = { 0, 0, 0, 0 };
		int                 _size = 4;
		
	};
		
	//-----------------------------------------------------------------------------
	// mvInputFloatMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvInputFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvInputFloatMulti : public mvFloat4PtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputFloatMulti, add_input_floatx)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvInputFloatMulti(mvUUID uuid);
		
		void setEnabled(bool value) override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		
	private:

		float               _min = 0.0f;
		float               _max = 100.0f;
		bool                _min_clamped = false;
		bool                _max_clamped = false;
		std::string         _format = "%.3f";
		ImGuiInputTextFlags _flags = 0;
		ImGuiInputTextFlags _stor_flags = 0;
		std::array<float, 4>_last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
		int                 _size = 4;
	};

}
