#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawImage, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvDrawImage : public mvAppItem, public mvEventHandler
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

		mvDrawImage(const std::string& name);
		~mvDrawImage() override;

		bool onEvent(mvEvent& event) override;
		bool onTextureDeleted(mvEvent& event);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;


	private:


		std::string m_file;
		mvVec2		m_pmax;
		mvVec2		m_pmin;
		mvVec2		m_uv_min;
		mvVec2		m_uv_max;
		mvColor		m_color;
		void* m_texture = nullptr;

	};

}
