#include "mvSimplePlot.h"
#include "mvItemRegistry.h"
//#include "mvImGuiThemeScope.h"
//#include "mvFontScope.h"

namespace Marvel {

	void mvSimplePlot::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "A simple plot for visualization of a 1 dimensional set of values.", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("default_value", mvArgType::KEYWORD_ARG, "()");
		parser.addArg<mvPyDataType::String>("overlay", mvArgType::KEYWORD_ARG, "''", "overlays text (similar to a plot title)");

		parser.addArg<mvPyDataType::Bool>("histogram", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "True");
		
		parser.addArg<mvPyDataType::Float>("min_scale", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Float>("max_scale", mvArgType::KEYWORD_ARG, "0.0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvSimplePlot::mvSimplePlot(mvUUID uuid)
		: mvFloatVectPtrBase(uuid)
	{
	}

	void mvSimplePlot::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::PushID(this);
		//mvImGuiThemeScope scope(this);
		//mvFontScope fscope(this);

		if (_histogram)
			ImGui::PlotHistogram(_label.c_str(), _value->data(), (int)_value->size(), 0, _overlay.c_str(),
				_min, _max, ImVec2((float)_width, (float)_height));
		else
			ImGui::PlotLines(_label.c_str(), _value->data(), (int)_value->size(), 0, _overlay.c_str(),
				_min, _max, ImVec2((float)_width, (float)_height));

		ImGui::PopID();
	}

	void mvSimplePlot::setPyValue(PyObject* value)
	{
		*_value = ToFloatVect(value);

		if (!_autosize)
			return;
		if (!_value->empty())
		{
			_max = _value->data()[0];
			_min = _value->data()[0];

			for (auto& item : *_value)
			{
				if (item > _max)
					_max = item;
				if (item < _min)
					_min = item;
			}
		}
	}

	void mvSimplePlot::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "overlay")) _overlay = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "minscale")) _min = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "maxscale")) _max = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "histogram")) _histogram = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "autosize")) _autosize = ToBool(item);
	}

	void mvSimplePlot::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "overlay", ToPyString(_overlay));
		PyDict_SetItemString(dict, "minscale", ToPyFloat(_min));
		PyDict_SetItemString(dict, "maxscale", ToPyFloat(_max));
		PyDict_SetItemString(dict, "histogram", ToPyBool(_histogram));
		PyDict_SetItemString(dict, "autosize", ToPyBool(_autosize));
	}

}