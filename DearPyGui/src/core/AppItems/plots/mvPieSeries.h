#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPieSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvPieSeries : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPieSeries, add_pie_series)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvPieSeries(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		double _x = 0.0;
		double _y = 0.0;
		double _radius = 0.5;
		bool _normalize = true;
		double _angle = 90.0;
		std::string _format;
		std::vector<std::string> _labels;
		std::vector<const char*> _clabels;

		mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
			std::vector<std::vector<double>>{ std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{} });

	};

}