#include <utility>
#include "mvPieSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

PyObject* mvPieSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvPieSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvPieSeries::setDataSource(mvUUID dataSource)
{
	if (dataSource == config.source) return;
	config.source = dataSource;

	mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
	if (!item)
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
			"Source item not found: " + std::to_string(dataSource), this);
		return;
	}
	if (DearPyGui::GetEntityValueType(item->type) != DearPyGui::GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
}

void mvPieSeries::draw(ImDrawList* drawlist, float x, float y)
{

	//-----------------------------------------------------------------------------
	// pre draw
	//-----------------------------------------------------------------------------
	if (!config.show)
		return;

	// push font if a font object is attached
	if (font)
	{
		ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
		ImGui::PushFont(fontptr);
	}

	// themes
	apply_local_theming(this);

	//-----------------------------------------------------------------------------
	// draw
	//-----------------------------------------------------------------------------
	{

		static const std::vector<double>* xptr;

		xptr = &(*_value.get())[0];

		ImPlot::PlotPieChart(_clabels.data(), xptr->data(), (int)_labels.size(),
			_x, _y, _radius, _normalize, _format.c_str(), _angle);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(info.internalLabel.c_str(), 1))
		{
			for (auto& childset : childslots)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->config.show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					UpdateAppItemState(item->state);
				}
			}
			ImPlot::EndLegendPopup();
		}
	}

	//-----------------------------------------------------------------------------
	// update state
	//   * only update if applicable
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// post draw
	//-----------------------------------------------------------------------------

	// pop font off stack
	if (font)
		ImGui::PopFont();

	// handle popping themes
	cleanup_local_theming(this);

}

void mvPieSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_x = ToDouble(PyTuple_GetItem(dict, 0));
	_y = ToDouble(PyTuple_GetItem(dict, 1));
	_radius = ToDouble(PyTuple_GetItem(dict, 2));
	(*_value)[0] = ToDoubleVect(PyTuple_GetItem(dict, 3));
	_labels = ToStringVect(PyTuple_GetItem(dict, 4));
	_clabels.clear();
	for (const auto& label : _labels)
		_clabels.push_back(label.c_str());

}

void mvPieSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "x")) _x = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(dict, "y")) _y = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(dict, "angle")) _angle = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(dict, "normalize")) _normalize = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "labels"))
	{
		_labels = ToStringVect(item);
		_clabels.clear();
		for (const auto& label : _labels)
			_clabels.push_back(label.c_str());
	}

	if (PyObject* item = PyDict_GetItemString(dict, "values")) { (*_value)[0] = ToDoubleVect(item); }


}

void mvPieSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_format = ToPyString(_format);
	mvPyObject py_x = ToPyDouble(_x);
	mvPyObject py_y = ToPyDouble(_y);
	mvPyObject py_radius = ToPyDouble(_radius);
	mvPyObject py_angle = ToPyDouble(_angle);
	mvPyObject py_normalize = ToPyBool(_normalize);
	mvPyObject py_labels = ToPyList(_labels);

	PyDict_SetItemString(dict, "format", py_format);
	PyDict_SetItemString(dict, "x", py_x);
	PyDict_SetItemString(dict, "y", py_y);
	PyDict_SetItemString(dict, "radius", py_radius);
	PyDict_SetItemString(dict, "angle", py_angle);
	PyDict_SetItemString(dict, "normalize", py_normalize);
	PyDict_SetItemString(dict, "labels", py_labels);
}

void mvPieSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvPieSeries*>(item);
	if (config.source != 0) _value = titem->_value;
	_x = titem->_x;
	_y = titem->_y;
	_radius = titem->_radius;
	_normalize = titem->_normalize;
	_angle = titem->_angle;
	_format = titem->_format;
	_labels = titem->_labels;
	_clabels = titem->_clabels;
}