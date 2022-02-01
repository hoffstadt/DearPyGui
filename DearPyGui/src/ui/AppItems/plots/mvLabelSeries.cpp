#include <utility>
#include "mvLabelSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/mvFontItems.h"
#include "AppItems/mvThemes.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

PyObject* mvLabelSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvLabelSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvLabelSeries::setDataSource(mvUUID dataSource)
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

void mvLabelSeries::draw(ImDrawList* drawlist, float x, float y)
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
		static const std::vector<double>* yptr;

		xptr = &(*_value.get())[0];
		yptr = &(*_value.get())[1];

		ImPlot::PlotText(info.internalLabel.c_str(), (*xptr)[0], (*yptr)[0], _vertical,
			ImVec2((float)_xoffset, (float)_yoffset));

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

void mvLabelSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	(*_value)[0] = ToDoubleVect(PyTuple_GetItem(dict, 0));
	(*_value)[1] = ToDoubleVect(PyTuple_GetItem(dict, 1));
}

void mvLabelSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "vertical")) _vertical = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "x_offset")) _xoffset = ToInt(item);
	if (PyObject* item = PyDict_GetItemString(dict, "y_offset")) _yoffset = ToInt(item);

	if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
	if (PyObject* item = PyDict_GetItemString(dict, "y")) { (*_value)[1] = ToDoubleVect(item); }

}

void mvLabelSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	mvPyObject py_vertical = ToPyBool(_vertical);
	mvPyObject py_x_offset = ToPyInt(_xoffset);
	mvPyObject py_y_offset = ToPyInt(_yoffset);

	PyDict_SetItemString(dict, "vertical", py_vertical);
	PyDict_SetItemString(dict, "x_offset", py_x_offset);
	PyDict_SetItemString(dict, "y_offset", py_y_offset);
}

void mvLabelSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvLabelSeries*>(item);
	if (config.source != 0) _value = titem->_value;
	_xoffset = titem->_xoffset;
	_yoffset = titem->_yoffset;
	_vertical = titem->_vertical;
}