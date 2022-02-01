#include <utility>
#include "mvHistogramSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"

PyObject* mvHistogramSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvHistogramSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvHistogramSeries::setDataSource(mvUUID dataSource)
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

void mvHistogramSeries::draw(ImDrawList* drawlist, float x, float y)
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

		ImPlot::PlotHistogram(info.internalLabel.c_str(), xptr->data(), (int)xptr->size(), _bins,
			_cumlative, _density, ImPlotRange(_min, _max), _outliers, (double)_barScale);

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

void mvHistogramSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	(*_value)[0] = ToDoubleVect(PyTuple_GetItem(dict, 0));
}

void mvHistogramSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "bins")) { _bins = ToInt(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "bar_scale")) { _barScale = ToFloat(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "min_range")) { _min = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "max_range")) { _max = ToDouble(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "cumlative")) { _cumlative = ToBool(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "density")) { _density = ToBool(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "outliers")) { _outliers = ToBool(item); }

}

void mvHistogramSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
}

void mvHistogramSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvHistogramSeries*>(item);
	_cumlative = titem->_cumlative;
	if (config.source != 0) _value = titem->_value;
	_bins = titem->_bins;
	_density = titem->_density;
	_outliers = titem->_outliers;
	_min = titem->_min;
	_max = titem->_max;
	_barScale = titem->_barScale;

}