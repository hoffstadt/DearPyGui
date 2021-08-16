#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "mvApp.h"
#include "mvModule_DearPyGui.h"

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
	class mvInputIntMulti : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputIntMulti, add_input_intx)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvInputIntMulti(mvUUID uuid);
		
		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:
		mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
		int                       _disabled_value[4]{};
		int                       _min = 0;
		int                       _max = 100;
		bool                      _min_clamped = false;
		bool                      _max_clamped = false;
		ImGuiInputTextFlags       _flags = 0;
		ImGuiInputTextFlags       _stor_flags = 0;
		std::array<int, 4>        _last_value = { 0, 0, 0, 0 };
		int                       _size = 4;
		
	};
		
	//-----------------------------------------------------------------------------
	// mvInputFloatMulti
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvInputFloatMulti, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 1);
	class mvInputFloatMulti : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputFloatMulti, add_input_floatx)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvInputFloatMulti(mvUUID uuid);
		
		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;
		
	private:

		mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
		float                       _disabled_value[4]{};
		float                       _min = 0.0f;
		float                       _max = 100.0f;
		bool                        _min_clamped = false;
		bool                        _max_clamped = false;
		std::string                 _format = "%.3f";
		ImGuiInputTextFlags         _flags = 0;
		ImGuiInputTextFlags         _stor_flags = 0;
		std::array<float, 4>        _last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
		int                         _size = 4;
	};

}
