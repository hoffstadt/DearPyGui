#pragma once

#include <utility>
#include <array>
#include "mvAppItem.h"

namespace Marvel {

	struct mvImageButtonConfig : public mvAppItemConfig
	{
		mvColor tint_color{ 255, 255, 255, 255 };
		mvColor background_color{ 0, 0, 0, 0 };
		int frame_padding = -1;
		std::array<float, 2> uv_min{ 0.0, 0.0 };
		std::array<float, 2> uv_max{ 1.0, 1.0 };
	};

	PyObject* add_image_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvImageButton : public mvAppItem, public mvEventHandler
	{

		MV_APPITEM_TYPE(mvAppItemType::ImageButton, mvImageButton, "add_image_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_Bg				, 21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_BgHovered		, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_BgActive		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeStyle_ImageButton_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeStyle_ImageButton_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeStyle_ImageButton_PaddingY		, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ImageButton_BorderSize, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ImageButton_PaddingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ImageButton_PaddingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvImageButton(const std::string& name, std::string  default_value);

		~mvImageButton() override;

		bool onEvent(mvEvent& event) override;
		bool onTextureDeleted(mvEvent& event);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::string m_value;
		mvVec2	    m_uv_min = { 0.0f, 0.0f };
		mvVec2	    m_uv_max = { 1.0f, 1.0f };
		mvColor     m_tintColor = { 255, 255, 255, 255, true };
		mvColor     m_backgroundColor = { 0, 0, 0, 0, true };
		void*       m_texture = nullptr;
		int         m_framePadding = -1;
		bool        m_dirty = false;

	};

}