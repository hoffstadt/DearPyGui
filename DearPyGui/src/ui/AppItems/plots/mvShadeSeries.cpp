#include <utility>
#include "mvShadeSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	mvShadeSeries::mvShadeSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvShadeSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvShadeSeries*>(item);
		if (config.source != 0) _value = titem->_value;
	}

	PyObject* mvShadeSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvShadeSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvShadeSeries::setDataSource(mvUUID dataSource)
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

	void mvShadeSeries::draw(ImDrawList* drawlist, float x, float y)
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
			static const std::vector<double>* y1ptr;
			static const std::vector<double>* y2ptr;

			xptr = &(*_value.get())[0];
			y1ptr = &(*_value.get())[1];
			y2ptr = &(*_value.get())[2];

			ImPlot::PlotShaded(info.internalLabel.c_str(), xptr->data(), y1ptr->data(),
				y2ptr->data(), (int)xptr->size());

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
		// 
		// pop font off stack
		if (font)
			ImGui::PopFont();

		// handle popping themes
		cleanup_local_theming(this);

	}

	void mvShadeSeries::handleSpecificRequiredArgs(PyObject* dict)
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
				(*_value)[2] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}

		for (auto& item : (*_value)[2])
			item = 0.0;
	}

	void mvShadeSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y1")) { valueChanged = true; (*_value)[1] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y2")) { valueChanged = true; (*_value)[2] = ToDoubleVect(item); }

		if (valueChanged)
		{
			if ((*_value)[1].size() != (*_value)[2].size())
			{
				(*_value)[2].clear();
				for(size_t i = 0; i < (*_value)[1].size(); i++)
                    (*_value)[2].push_back(0.0);

			}
		}
	}

	void mvShadeSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}