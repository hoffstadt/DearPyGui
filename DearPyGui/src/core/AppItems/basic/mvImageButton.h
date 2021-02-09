#pragma once

#include <utility>
#include "mvAppItem.h"

namespace Marvel {

	PyObject* add_image_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvImageButton : public mvAppItem, public mvEventHandler
	{

		MV_APPITEM_TYPE(mvAppItemType::ImageButton, mvImageButton, "add_image_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_Bg				, 21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_BgHovered		, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_BgActive		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ImageButton, mvThemeCol_ImageButton_BorderShadow	,  6L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ImageButton_BorderShadow),
		MV_END_COLOR_CONSTANTS

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