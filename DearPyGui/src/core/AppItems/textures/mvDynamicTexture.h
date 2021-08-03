#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDynamicTexture, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvDynamicTexture : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDynamicTexture, add_dynamic_texture)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDynamicTexture(mvUUID uuid);
		~mvDynamicTexture();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void setWidth(int width) override {}
		void setHeight(int height) override {}
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void* getRawTexture() { return _texture; }

	private:

		mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
		void*                     _texture = nullptr;
		bool                      _dirty = true;

	};

}
