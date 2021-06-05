#pragma once

#include "mvPlot.h"
#include "mvEvents.h"
#include "textures/mvStaticTexture.h"
#include "textures/mvDynamicTexture.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvImageSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvImageSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvImageSeries, add_image_series)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvImageSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		std::string m_imagevalue;
		mvPlotPoint m_bounds_min = {0.0, 0.0};
		mvPlotPoint m_bounds_max = {0.0, 0.0};
		mvVec2	    m_uv_min = { 0.0f, 0.0f };
		mvVec2	    m_uv_max = { 1.0f, 1.0f };
		mvColor     m_tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<mvAppItem> m_texture = nullptr;

	};

}
