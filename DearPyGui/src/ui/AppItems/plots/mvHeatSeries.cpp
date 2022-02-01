#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

PyObject* mvHeatSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvHeatSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvHeatSeries::setDataSource(mvUUID dataSource)
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

void mvHeatSeries::draw(ImDrawList* drawlist, float x, float y)
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


		ImPlot::PlotHeatmap(info.internalLabel.c_str(), xptr->data(), _rows, _cols, _scale_min, _scale_max,
			_format.c_str(), { _bounds_min.x, _bounds_min.y }, { _bounds_max.x, _bounds_max.y });

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

void mvHeatSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	(*_value)[0] = ToDoubleVect(PyTuple_GetItem(dict, 0));
	_rows = ToInt(PyTuple_GetItem(dict, 1));
	_cols = ToInt(PyTuple_GetItem(dict, 2));
	(*_value)[1].push_back(_bounds_min.y);
	(*_value)[1].push_back(_bounds_max.y);
}

void mvHeatSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "format")) _format = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "rows")) _rows = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "cols")) _cols = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "bounds_min")) _bounds_min = ToPoint(item);
	if (PyObject* item = PyDict_GetItemString(dict, "bounds_max")) _bounds_max = ToPoint(item);
	if (PyObject* item = PyDict_GetItemString(dict, "scale_min")) _scale_min = ToDouble(item);
	if (PyObject* item = PyDict_GetItemString(dict, "scale_max")) _scale_max = ToDouble(item);

	bool valueChanged = false;
	if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*_value)[0] = ToDoubleVect(item); }

	if (valueChanged)
	{
		(*_value)[1].push_back(_bounds_min.y);
		(*_value)[1].push_back(_bounds_max.y);
	}

}

void mvHeatSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_format = ToPyString(_format);
	mvPyObject py_rows = ToPyInt(_rows);
	mvPyObject py_cols = ToPyInt(_cols);
	mvPyObject py_bounds_min = ToPyPair(_bounds_min.x, _bounds_min.y);
	mvPyObject py_bounds_max = ToPyPair(_bounds_max.x, _bounds_max.y);
	mvPyObject py_scale_min = ToPyDouble(_scale_min);
	mvPyObject py_scale_max = ToPyDouble(_scale_max);

	PyDict_SetItemString(dict, "format", py_format);
	PyDict_SetItemString(dict, "rows", py_rows);
	PyDict_SetItemString(dict, "cols", py_cols);
	PyDict_SetItemString(dict, "bounds_min", py_bounds_min);
	PyDict_SetItemString(dict, "bounds_max", py_bounds_max);
	PyDict_SetItemString(dict, "scale_min", py_scale_min);
	PyDict_SetItemString(dict, "scale_max", py_scale_max);
}

void mvHeatSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvHeatSeries*>(item);
	if (config.source != 0) _value = titem->_value;
	_rows = titem->_rows;
	_cols = titem->_cols;
	_scale_min = titem->_scale_min;
	_scale_max = titem->_scale_max;
	_format = titem->_format;
	_bounds_min = titem->_bounds_min;
	_bounds_max = titem->_bounds_max;
}