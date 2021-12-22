#include <utility>
#include "mvErrorSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	mvErrorSeries::mvErrorSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvErrorSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvErrorSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvErrorSeries::setDataSource(mvUUID dataSource)
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

	void mvErrorSeries::draw(ImDrawList* drawlist, float x, float y)
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
			static const std::vector<double>* zptr;
			static const std::vector<double>* wptr;

			xptr = &(*_value.get())[0];
			yptr = &(*_value.get())[1];
			zptr = &(*_value.get())[2];
			wptr = &(*_value.get())[3];

			if (_horizontal)
				ImPlot::PlotErrorBarsH(info.internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());
			else
				ImPlot::PlotErrorBars(info.internalLabel.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());

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

	void mvErrorSeries::handleSpecificRequiredArgs(PyObject* dict)
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

			case 1:
				(*_value)[1] = ToDoubleVect(item);
				break;

			case 2:
				(*_value)[2] = ToDoubleVect(item);
				break;

			case 3:
				(*_value)[3] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}
	}

	void mvErrorSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) _horizontal= ToBool(item);

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { (*_value)[1] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "negative")) { (*_value)[2] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "positive")) { (*_value)[3] = ToDoubleVect(item); }

	}

	void mvErrorSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_horizontal = ToPyBool(_horizontal);

		PyDict_SetItemString(dict, "horizontal", py_horizontal);
	}

	void mvErrorSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvErrorSeries*>(item);
		if (config.source != 0) _value = titem->_value;
		_horizontal = titem->_horizontal;
	}
}