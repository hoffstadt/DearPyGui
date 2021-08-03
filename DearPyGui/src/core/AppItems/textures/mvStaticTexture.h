#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStaticTexture, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvStaticTexture : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStaticTexture, add_static_texture)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvStaticTexture(mvUUID uuid);
		~mvStaticTexture();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void setWidth(int width) override {}
		void setHeight(int height) override {}
		void* getRawTexture() { return _texture; }
		void markDirty() { _dirty = true; }

	private:

		mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});
		void*                     _texture = nullptr;
		bool                      _dirty = true;

	};

}
