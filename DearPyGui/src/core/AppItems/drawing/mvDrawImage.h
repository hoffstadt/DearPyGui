#pragma once

#include "mvAppItem.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDrawImage);
	class mvDrawImage : public mvAppItem, public mvEventHandler
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvDrawImage, draw_image)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDrawImage(const std::string& name, std::string file, const mvVec2& pmin, const mvVec2& pmax, const mvVec2& uv_min,
			const mvVec2& uv_max, const mvColor& color);
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
		int         m_width = 0;
		int         m_height = 0;

	};

}
