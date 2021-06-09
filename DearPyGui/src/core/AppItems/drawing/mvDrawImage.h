#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawImage, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 2);
	class mvDrawImage : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDrawImage, draw_image)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawImage(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:


		mvUUID      m_file = 0;
		mvVec2		m_pmax = {0.0f, 0.0f};
		mvVec2		m_pmin = {0.0f, 0.0f};
		mvVec2		m_uv_min = {0.0f, 0.0f};
		mvVec2		m_uv_max = {1.0f, 1.0f};
		mvColor		m_color = mvImGuiCol_Text;
		std::shared_ptr<mvAppItem> m_texture = nullptr;

	};

}
