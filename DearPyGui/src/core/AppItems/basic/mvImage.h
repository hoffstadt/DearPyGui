#pragma once

#include <utility>
#include <array>
#include "mvAppItem.h"

namespace Marvel {

	struct mvImageConfig : public mvAppItemConfig
	{
		
		std::string value = "";
		mvColor tint_color = mvColor{ 255, 255, 255, 255 };
		mvColor border_color = mvColor{ 0, 0, 0, 0 };
		std::array<float, 2> uv_min{ 0.0, 0.0 };
		std::array<float, 2> uv_max{ 1.0, 1.0 };
	};

	

	MV_REGISTER_WIDGET(mvImage);
	class mvImage : public mvAppItem, public mvEventHandler
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvImage, add_image)

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvImage(const std::string& name, std::string default_value);

		bool onEvent(mvEvent& event) override;
		bool onTextureDeleted(mvEvent& event);

		~mvImage() override;

		void               draw              ()               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		void               setValue          (const std::string& value);
		const std::string& getValue          () const;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		std::string m_value;
		mvVec2	    m_uv_min = {0.0f, 0.0f};
		mvVec2	    m_uv_max = {1.0f, 1.0f};
		mvColor     m_tintColor = {1.0f, 1.0f, 1.0f, 1.0f};
		mvColor     m_borderColor = {0.0f, 0.0f, 0.0f, 0.0f};
		void*       m_texture = nullptr;
		bool        m_dirty = false;
		mvImageConfig m_config;

	};

}