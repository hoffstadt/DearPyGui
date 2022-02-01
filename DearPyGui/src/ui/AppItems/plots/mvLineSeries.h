#pragma once

#include "mvPlot.h"

class mvLineSeries : public mvAppItem
{

public:

	explicit mvLineSeries(mvUUID uuid) : mvAppItem(uuid) {}

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

	mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
		std::vector<std::vector<double>>{ std::vector<double>{},
		std::vector<double>{},
		std::vector<double>{},
		std::vector<double>{},
		std::vector<double>{} });

};