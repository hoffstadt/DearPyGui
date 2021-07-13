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

		int                 m_min = 0;
		int                 m_max = 100;
		bool                m_min_clamped = false;
		bool                m_max_clamped = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;
		std::array<int, 4>  m_last_value = { 0, 0, 0, 0 };
		int                 m_size = 4;
		
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

		float               m_min = 0.0f;
		float               m_max = 100.0f;
		bool                m_min_clamped = false;
		bool                m_max_clamped = false;
		std::string         m_format = "%.3f";
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;
		std::array<float, 4>m_last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
		int                 m_size = 4;
	};

}
