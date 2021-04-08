#include "mvAreaSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvAreaSeries::drawPolygon()
	{

		static const std::vector<float>* xptr;
		static const std::vector<float>* yptr;

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];

		ImPlotLimits limits = ImPlot::GetPlotLimits();

		std::vector<ImVec2> points;
		for (unsigned i = 0; i < xptr->size(); i++)
		{
			float x = (*xptr)[i] > limits.X.Max ? (float)limits.X.Max : (float)(*xptr)[i];
			x = (*xptr)[i] < limits.X.Min ? (float)limits.X.Min : x;

			float y = (*yptr)[i] > limits.Y.Max ? (float)limits.Y.Max : (float)(*yptr)[i];
			y = (*yptr)[i] < limits.Y.Min ? (float)limits.Y.Min : y;
			auto p = ImPlot::PlotToPixels({ x, y });
			points.push_back(p);
		}

		if (m_fill.r > 0.0f)
		{
			size_t i;
			int y;
			int miny, maxy;
			int x1, y1;
			int x2, y2;
			int ind1, ind2;
			size_t ints;
			size_t n = points.size();
			int* polyints = new int[n];

			/* Determine Y maxima */
			miny = (int)points[0].y;
			maxy = (int)points[0].y;
			for (i = 1; i < n; i++)
			{
				miny = std::min(miny, (int)points[i].y);
				maxy = std::max(maxy, (int)points[i].y);
			}

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
					ImGui::GetWindowDrawList()->AddLine({ (float)polyints[i], (float)y }, { (float)polyints[i + 1], (float)y }, m_fill, 1.0f);
			}
			delete[] polyints;
		}

	}

	void mvAreaSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("x");
		parser.addArg<mvPyDataType::FloatList>("y");

		parser.addArg<mvPyDataType::IntList>("fill", mvArgType::KEYWORD, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Integer>("axis", mvArgType::KEYWORD, "0");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvAreaSeries::mvAreaSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvAreaSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		switch (m_axis)
		{
		case ImPlotYAxis_1:
			ImPlot::SetPlotYAxis(ImPlotYAxis_1);
			break;
		case ImPlotYAxis_2:
			ImPlot::SetPlotYAxis(ImPlotYAxis_2);
			break;
		case ImPlotYAxis_3:
			ImPlot::SetPlotYAxis(ImPlotYAxis_3);
			break;
		default:
			break;
		}

		static const std::vector<float>* xptr;
		static const std::vector<float>* yptr;

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];

		ImPlot::PlotLine(m_label.c_str(), xptr->data(), yptr->data(), xptr->size());

		ImPlot::PushPlotClipRect();
		auto item = ImPlot::RegisterOrGetItem(m_label.c_str());
		drawPolygon();
		ImPlot::PopPlotClipRect();

	}

	void mvAreaSeries::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { valueChanged = true; (*m_value)[1] = ToFloatVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvAreaSeries::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}
