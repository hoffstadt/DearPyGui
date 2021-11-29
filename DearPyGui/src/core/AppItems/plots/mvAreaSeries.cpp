#include "mvAreaSeries.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvAreaSeries::drawPolygon()
	{

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*_value.get())[0];
		yptr = &(*_value.get())[1];

		std::vector<ImVec2> points;
		for (unsigned i = 0; i < xptr->size(); i++)
		{
			auto p = ImPlot::PlotToPixels({ (*xptr)[i], (*yptr)[i] });
			points.push_back(p);
		}

		if (_fill.r > 0.0f)
		{
			size_t i;
			double y;
			double miny, maxy;
			double x1, y1;
			double x2, y2;
			int ind1, ind2;
			size_t ints;
			size_t n = points.size();
			int* polyints = new int[n];


			/* Get plot Y range in pixels */
			ImPlotLimits limits = ImPlot::GetPlotLimits();
			auto upperLimitsPix = ImPlot::PlotToPixels({ limits.X.Max, limits.Y.Max });
			auto lowerLimitsPix = ImPlot::PlotToPixels({ limits.X.Min, limits.Y.Min });

			/* Determine Y range of data*/
			miny = (int)points[0].y;
			maxy = (int)points[0].y;
			for (i = 1; i < n; i++)
			{
				miny = std::min((int)miny, (int)points[i].y);
				maxy = std::max((int)maxy, (int)points[i].y);
			}

			/* Determine to clip scans based on plot bounds y or data bounds y
			when the plot data is converted the min and max y invert (due to plot to graphics coord) 
			so we comapre min with max and max with min*/
			miny = std::max((int)miny, (int)upperLimitsPix.y);
			maxy = std::min((int)maxy, (int)lowerLimitsPix.y);

			/* Draw, scanning y */
			for (y = miny; y <= maxy; y++) {
				ints = 0;
				for (i = 0; (i < n); i++) {
					if (!i)
					{
						ind1 = (int)n - 1;
						ind2 = 0;
					}
					else
					{
						ind1 = (int)i - 1;
						ind2 = (int)i;
					}
					y1 = (int)points[ind1].y;
					y2 = (int)points[ind2].y;
					if (y1 < y2)
					{
						x1 = (int)points[ind1].x;
						x2 = (int)points[ind2].x;
					}
					else if (y1 > y2)
					{
						y2 = (int)points[ind1].y;
						y1 = (int)points[ind2].y;
						x2 = (int)points[ind1].x;
						x1 = (int)points[ind2].x;
					}
					else
						continue;

					if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
						polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;

				}

				auto compare_int = [](const void* a, const void* b)
				{
					return (*(const int*)a) - (*(const int*)b);
				};

				qsort(polyints, ints, sizeof(int), compare_int);

				for (i = 0; i < ints; i += 2)
					ImGui::GetWindowDrawList()->AddLine({ (float)polyints[i], (float)y }, { (float)polyints[i + 1], (float)y }, _fill, 1.0f);
			}
			delete[] polyints;
		}

	}

	void mvAreaSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::DoubleList, "x" });
		args.push_back({ mvPyDataType::DoubleList, "y" });
		args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
		args.push_back({ mvPyDataType::Bool, "contribute_to_bounds", mvArgType::KEYWORD_ARG, "True" });

		mvPythonParserSetup setup;
		setup.about = "Adds an area series to a plot.";
		setup.category = { "Plotting", "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvAreaSeries::mvAreaSeries(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvAreaSeries::getPyValue()
	{
		return ToPyList(*_value);
	}

	void mvAreaSeries::setPyValue(PyObject* value)
	{
		*_value = ToVectVectDouble(value);
	}

	void mvAreaSeries::setDataSource(mvUUID dataSource)
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
		if (GetEntityValueType(item->getType()) != GetEntityValueType(getType()))
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::vector<std::vector<double>>>*>(item->getValue());
	}

	void mvAreaSeries::draw(ImDrawList* drawlist, float x, float y)
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
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
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

			ImPlot::PlotLine(_internalLabel.c_str(), xptr->data(), yptr->data(), (int)xptr->size());

			ImPlot::PushPlotClipRect();
			ImPlot::RegisterOrGetItem(_internalLabel.c_str());
			drawPolygon();
			ImPlot::PopPlotClipRect();

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
		if (auto classTheme = getClassThemeComponent())
			static_cast<mvThemeComponent*>(classTheme.get())->customAction();

		if (_theme)
		{
			static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
			static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
			static_cast<mvTheme*>(_theme.get())->customAction();
		}

	}

	void mvAreaSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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

			default:
				break;
			}
		}
	}

	void mvAreaSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { (*_value)[1] = ToDoubleVect(item); }

	}

	void mvAreaSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_fill = ToPyColor(_fill);

		PyDict_SetItemString(dict, "fill", py_fill);

	}

	void mvAreaSeries::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvAreaSeries*>(item);
		if(_source != 0) _value = titem->_value;
		_fill = titem->_fill;
	}

}
