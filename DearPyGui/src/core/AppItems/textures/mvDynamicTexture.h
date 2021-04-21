#pragma once

#include "mvTypeBases.h"
#include "cpp.hint"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDynamicTexture, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvDynamicTexture : public mvFloatVectPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDynamicTexture, add_dynamic_texture)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDynamicTexture(const std::string& name);
		~mvDynamicTexture();

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void setWidth(int width) override {}
		void setHeight(int height) override {}

		void* getRawTexture() { return m_texture; }

	private:

		void* m_texture = nullptr;
		bool  m_dirty = true;

	};

}
