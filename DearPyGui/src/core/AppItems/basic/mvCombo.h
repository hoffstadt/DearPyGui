#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCombo, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvCombo : public mvAppItem
	{

		enum class ComboHeightMode {
			mvComboHeight_Small = 0L,
			mvComboHeight_Regular,
			mvComboHeight_Large,
			mvComboHeight_Largest
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCombo, add_combo)

		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN

		MV_CREATE_CONSTANT(mvComboHeight_Small, 0L);
		MV_CREATE_CONSTANT(mvComboHeight_Regular, 1L);
		MV_CREATE_CONSTANT(mvComboHeight_Large, 2L);
		MV_CREATE_CONSTANT(mvComboHeight_Largest, 3L);

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvComboHeight_Small),
			MV_ADD_CONSTANT(mvComboHeight_Regular),
			MV_ADD_CONSTANT(mvComboHeight_Large),
			MV_ADD_CONSTANT(mvComboHeight_Largest)
		MV_END_CONSTANTS

	public:

		explicit mvCombo(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		mvRef<std::string>       _value = CreateRef<std::string>("");
		std::string              _disabled_value = "";
		ImGuiComboFlags          _flags = ImGuiComboFlags_None;
		std::vector<std::string> _items;
		bool                     _popup_align_left = false;
		bool                     _no_preview = false;
	};

}
