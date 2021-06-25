#pragma once

#include <utility>
#include <array>
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"
#include "textures/mvRawTexture.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvImageButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvImageButton : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvImageButton, add_image_button)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvImageButton(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		// config
		mvUUID      m_textureUUID = 0;
		mvVec2	    m_uv_min = { 0.0f, 0.0f };
		mvVec2	    m_uv_max = { 1.0f, 1.0f };
		mvColor     m_tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		mvColor     m_backgroundColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		int         m_framePadding = -1;

		// pointer to existing item or internal
		std::shared_ptr<mvAppItem> m_texture = nullptr;
		bool m_internalTexture = false; // create a local texture if necessary

	};

}