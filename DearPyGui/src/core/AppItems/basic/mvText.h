#pragma once

#include "mvItemRegistry.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvText
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvText, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvText : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvText, add_text)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvText(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		mvRef<std::string> _value = CreateRef<std::string>("");
		std::string        _disabled_value = "";
		mvColor            _color = {-1.0f, 0.0f, 0.0f, 1.0f};
		int                _wrap = -1;
		bool               _bullet = false;
		bool	           _show_label = false;

	};
}