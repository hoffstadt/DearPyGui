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

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvDrawImage(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;


	private:


		mvUUID      m_textureUUID = 0;
		mvVec2		m_pmax = {0.0f, 0.0f};
		mvVec2		m_pmin = {0.0f, 0.0f};
		mvVec2		m_uv_min = {0.0f, 0.0f};
		mvVec2		m_uv_max = {1.0f, 1.0f};
		mvColor		m_color = mvImGuiCol_Text;

		// pointer to existing item or internal
		std::shared_ptr<mvAppItem> m_texture = nullptr;
		bool m_internalTexture = false; // create a local texture if necessary

	};

}
