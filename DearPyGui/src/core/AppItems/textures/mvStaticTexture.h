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

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvStaticTexture(const std::string& name);
		~mvStaticTexture();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void setWidth(int width) override {}
		void setHeight(int height) override {}

		void* getRawTexture() { return m_texture; }
		void markDirty() { m_dirty = true; }

	private:

		void* m_texture = nullptr;
		bool  m_dirty = true;

	};

}
