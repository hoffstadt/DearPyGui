#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvLabelSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvLabelSeries : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvLabelSeries, add_text_point)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvPlotAxis),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
		MV_END_PARENTS

	public:

		mvLabelSeries(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		int  _xoffset = 0;
		int  _yoffset = 0;
		bool _vertical = false;
		mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
			std::vector<std::vector<double>>{ std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{} });

	};

}
