#include <utility>
#include "mv2dHistogramSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	mv2dHistogramSeries::mv2dHistogramSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mv2dHistogramSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mv2dHistogramSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mv2dHistogramSeries::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = GetItem((*GContext->itemRegistry), dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (GetEntityValueType(item->_type) != GetEntityValueType(_type))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
	}

	void mv2dHistogramSeries::draw(ImDrawList* drawlist, float x, float y)
	{


		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------
		if (!_show)
			return;

		// push font if a font object is attached
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// themes
		if (auto classTheme = GetClassThemeComponent(_type))
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)_type);
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{

			static const std::vector<double>* xptr;
			static const std::vector<double>* yptr;

			xptr = &(*_value.get())[0];
			yptr = &(*_value.get())[1];

			ImPlot::PlotHistogram2D(_internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size(),
				_xbins, _ybins, _density, ImPlotLimits(_xmin, _xmax, _ymin, _ymax), _outliers);

			// Begin a popup for a legend entry.
			if (ImPlot::BeginLegendPopup(_internalLabel.c_str(), 1))
			{
				for (auto& childset : _children)
				{
					for (auto& item : childset)
					{
						// skip item if it's not shown
						if (!item->_show)
							continue;
						item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
						UpdateAppItemState(item->_state);
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
		if (_font)
			ImGui::PopFont();

		// handle popping themes
		if (auto classTheme = GetClassThemeComponent(_type))
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)_type);
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

	}

	void mv2dHistogramSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(_type)], dict))
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

	void mv2dHistogramSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "xbins")) { _xbins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "ybins")) { _ybins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "xmin_range")) { _xmin = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "xmax_range")) { _xmax = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "ymin_range")) { _ymin = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "ymax_range")) { _ymax = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "density")) { _density = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "outliers")) { _outliers = ToBool(item); }

	}

	void mv2dHistogramSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	void mv2dHistogramSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mv2dHistogramSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_xbins = titem->_xbins;
		_ybins = titem->_ybins;
		_density = titem->_density;
		_outliers = titem->_outliers;
		_xmin = titem->_xmin;
		_xmax = titem->_xmax;
		_ymin = titem->_ymin;
		_ymax = titem->_ymax;
	}

}