# pragma once

#include "Core/PythonUtilities/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel{

	static std::map<std::string, mvPythonTranslator> BuildTranslations()
	{
		std::map<std::string, mvPythonTranslator> translators = {

			{"addPlot", mvPythonTranslator({
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::String, "xAxisName"},
				{mvPythonDataType::String, "yAxisName"},
				{mvPythonDataType::Integer, "width"},
				{mvPythonDataType::Integer, "height"},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "flags"},
				{mvPythonDataType::Integer, "xflags"},
				{mvPythonDataType::Integer, "yflags"}
			}, false, "Needs documentation")},

			{"clearPlot", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
			}, false, "Needs documentation")},

			{"setColorMap", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::Integer, "map"}
			}, false, "Needs documentation")},

			{"addLineSeries", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "data"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::FloatList, "color"},
				{mvPythonDataType::Float, "weight"}
			}, false, "Needs documentation")},

			{"addScatterSeries", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::FloatList, "data"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Integer, "marker"},
				{mvPythonDataType::Float, "size"},
				{mvPythonDataType::Float, "weight"},
				{mvPythonDataType::FloatList, "outline"},
				{mvPythonDataType::FloatList, "fill"},
			}, false, "Needs documentation")},

			{"addTextPoint", mvPythonTranslator({
				{mvPythonDataType::String, "plot"},
				{mvPythonDataType::String, "name"},
				{mvPythonDataType::Float, "x"},
				{mvPythonDataType::Float, "y"},
				{mvPythonDataType::Optional},
				{mvPythonDataType::KeywordOnly},
				{mvPythonDataType::Bool, "vertical"},
				{mvPythonDataType::Integer, "xoffset"},
				{mvPythonDataType::Integer, "yoffset"}
			}, false, "Needs documentation")}

		};

		return translators;
	}

}