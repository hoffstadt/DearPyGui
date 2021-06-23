#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvAnnotation, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Float4, 0);
	class mvAnnotation : public mvDouble4PtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvAnnotation, add_plot_annotation)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

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
