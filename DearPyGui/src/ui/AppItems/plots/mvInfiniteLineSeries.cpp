#include <utility>
#include "mvInfiniteLineSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

mvVLineSeries::mvVLineSeries(mvUUID uuid)
	: mvAppItem(uuid)
{
}

PyObject* mvVLineSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvVLineSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvVLineSeries::setDataSource(mvUUID dataSource)
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
	if (GetEntityValueType(item->type) != GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
}

void mvVLineSeries::draw(ImDrawList* drawlist, float x, float y)
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

		ImPlot::PlotVLines(info.internalLabel.c_str(), xptr->data(), (int)xptr->size());

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

void mvVLineSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	for (int i = 0; i < PyTuple_Size(dict); i++)
	{
		PyObject* item = PyTuple_GetItem(dict, i);
		switch (i)
		{
		case 0:
			(*_value)[0] = ToDoubleVect(item);
			break;

		default:
			break;
		}
	}
}

void mvVLineSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }


}

void mvVLineSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
}

void mvVLineSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvVLineSeries*>(item);
	if (config.source != 0) _value = titem->_value;
}

mvHLineSeries::mvHLineSeries(mvUUID uuid)
	: mvAppItem(uuid)
{
}

PyObject* mvHLineSeries::getPyValue()
{
	return ToPyList(*_value);
}

void mvHLineSeries::setPyValue(PyObject* value)
{
	*_value = ToVectVectDouble(value);
}

void mvHLineSeries::setDataSource(mvUUID dataSource)
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
	if (GetEntityValueType(item->type) != GetEntityValueType(type))
	{
		mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
			"Values types do not match: " + std::to_string(dataSource), this);
		return;
	}
	_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
}

void mvHLineSeries::draw(ImDrawList* drawlist, float x, float y)
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

		ImPlot::PlotHLines(info.internalLabel.c_str(), xptr->data(), (int)xptr->size());

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

void mvHLineSeries::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	for (int i = 0; i < PyTuple_Size(dict); i++)
	{
		PyObject* item = PyTuple_GetItem(dict, i);
		switch (i)
		{
		case 0:
			(*_value)[0] = ToDoubleVect(item);
			break;

		default:
			break;
		}
	}
}

void mvHLineSeries::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	bool valueChanged = false;
	if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*_value)[0] = ToDoubleVect(item); }

}

void mvHLineSeries::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
}

void mvHLineSeries::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvHLineSeries*>(item);
	if (config.source != 0) _value = titem->_value;
}