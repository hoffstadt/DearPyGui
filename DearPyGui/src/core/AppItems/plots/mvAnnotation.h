#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvAnnotation, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 0);
	class mvAnnotation : public mvDouble4PtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvAnnotation, add_plot_annotation)
		
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Annotation, ImPlotCol_MarkerOutline, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Plot_Annotation, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvAnnotation(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		mvColor m_color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
		bool    m_clamped = true;
		ImVec2  m_pixOffset;

	};

}
