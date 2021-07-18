#pragma once

#include <utility>
#include "mvTypeBases.h"

namespace Marvel{

	MV_REGISTER_WIDGET(mvSimplePlot, MV_ITEM_DESC_DEFAULT, StorageValueTypes::FloatVect, 1);
	class mvSimplePlot : public mvFloatVectPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSimplePlot, add_simple_plot)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		explicit mvSimplePlot(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		std::string        m_overlay;
		float              m_min = 0.0f;
		float              m_max = 0.0f;
		bool               m_histogram = false;
		bool               m_autosize = true;

	};

}