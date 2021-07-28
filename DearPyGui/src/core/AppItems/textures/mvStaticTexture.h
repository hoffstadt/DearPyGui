#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStaticTexture, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvStaticTexture : public mvFloatVectPtrBase
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
		void setWidth(int width) override {}
		void setHeight(int height) override {}
		void* getRawTexture() { return _texture; }
		void markDirty() { _dirty = true; }

	private:

		void*       _texture = nullptr;
		bool        _dirty = true;

	};

}
