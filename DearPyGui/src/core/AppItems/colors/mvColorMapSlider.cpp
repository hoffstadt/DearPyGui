#include "mvColorMapSlider.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvColorMapSlider::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Adds a color button.", { "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Float>("default_value", mvArgType::KEYWORD_ARG, "0.0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvColorMapSlider::mvColorMapSlider(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvColorMapSlider::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvColorMapSlider*>(item);
		if (_source != 0) _value = titem->_value;
		_color = titem->_color;
		_colormap = titem->_colormap;

	}

	void mvColorMapSlider::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (ImPlot::ColormapSlider(_internalLabel.c_str(), _value.get(), &_color, "", _colormap))
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]() {
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), _uuid, ToPyFloat(*_value), _user_data);
				});

	}

	PyObject* mvColorMapSlider::getPyValue()
	{
		return ToPyFloat(*_value);
	}

	void mvColorMapSlider::setPyValue(PyObject* value)
	{
		*_value = ToFloat(value);
	}

	void mvColorMapSlider::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<float>*>(item->getValue());
	}

}