#include <utility>
#include "mvDragPoint.h"
#include "mvCore.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvDragPoint::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::DoubleList, "default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0)" });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
		args.push_back({ mvPyDataType::Bool, "show_label", mvArgType::KEYWORD_ARG, "True" });

		mvPythonParserSetup setup;
		setup.about = "Adds a drag point to a plot.";
		setup.category = { "Plotting", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDragPoint::mvDragPoint(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	PyObject* mvDragPoint::getPyValue()
	{
		return ToPyFloatList(_value->data(), 4);
	}

	void mvDragPoint::setPyValue(PyObject* value)
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

	void mvDragPoint::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::array<double, 4>>*>(item->getValue());
	}

	void mvDragPoint::draw(ImDrawList* drawlist, float x, float y)
	{
		if (!_show)
			return;

		ScopedID id(_uuid);

		static double dummyx = (*_value.get())[0];
		static double dummyy = (*_value.get())[1];
		dummyx = (*_value.get())[0];
		dummyy = (*_value.get())[1];

		if (ImPlot::DragPoint(_specifiedLabel.c_str(), &dummyx, &dummyy, _show_label, _color, _radius))
		{
			(*_value.get())[0] = dummyx;
			(*_value.get())[1] = dummyy;
			mvAddCallback(_callback, _uuid, nullptr, _user_data);
		}

	}

	void mvDragPoint::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show_label")) _show_label = ToBool(item);

	}

	void mvDragPoint::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_color = ToPyColor(_color);
		mvPyObject py_radius = ToPyFloat(_radius);
		mvPyObject py_show_label = ToPyBool(_show_label);

		PyDict_SetItemString(dict, "color", py_color);
		PyDict_SetItemString(dict, "radius", py_radius);
		PyDict_SetItemString(dict, "show_label", py_show_label);
	}

	void mvDragPoint::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDragPoint*>(item);
		if(_source != 0) _value = titem->_value;
		_disabled_value[0] = titem->_disabled_value[0];
		_disabled_value[1] = titem->_disabled_value[1];
		_disabled_value[2] = titem->_disabled_value[2];
		_disabled_value[3] = titem->_disabled_value[3];
		_show_label = titem->_show_label;
		_color = titem->_color;
		_radius = titem->_radius;
	}

}