#include <utility>
#include "mvAnnotation.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvAnnotation::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds an annotation to a plot.", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		parser.addArg<mvPyDataType::FloatList>("offset", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		
		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvAnnotation::mvAnnotation(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvAnnotation::getPyValue()
	{
		return ToPyFloatList(_value->data(), 4);
	}

	void mvAnnotation::setPyValue(PyObject* value)
	{
		std::vector<double> temp = ToDoubleVect(value);
		while (temp.size() < 4)
			temp.push_back(0.0);
		std::array<double, 4> temp_array;
		for (size_t i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (_value)
			*_value = temp_array;
		else
			_value = std::make_shared<std::array<double, 4>>(temp_array);
	}

	void mvAnnotation::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<std::array<double, 4>>*>(item->getValue());
	}

	void mvAnnotation::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (_clamped)
			ImPlot::AnnotateClamped((*_value.get())[0], (*_value.get())[1], _pixOffset, _color.toVec4(), "%s", _specificedlabel.c_str());
		else
			ImPlot::Annotate((*_value.get())[0], (*_value.get())[1], _pixOffset, _color.toVec4(), "%s", _specificedlabel.c_str());

	}

	void mvAnnotation::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "clamped")) _clamped = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "offset")) _pixOffset = ToVec2(item);

	}

	void mvAnnotation::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "clamped", ToPyBool(_clamped));
		PyDict_SetItemString(dict, "offset", ToPyPair(_pixOffset.x, _pixOffset.y));

	}

}