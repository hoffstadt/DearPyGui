#include "mvThemeColor.h"
#include "mvContext.h"
#include <array>
#include <implot.h>
#include <imnodes.h>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvThemeColor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		args.push_back({ mvPyDataType::Long, "target", mvArgType::POSITIONAL_ARG, "0" });
		args.push_back({ mvPyDataType::IntList, "value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)" });
		args.push_back({ mvPyDataType::Integer, "category", mvArgType::KEYWORD_ARG, "0", "Options include mvThemeCat_Core, mvThemeCat_Plots, mvThemeCat_Nodes." });

		mvPythonParserSetup setup;
		setup.about = "Adds a theme color.";
		setup.category = { "Themes" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });

	}

	mvThemeColor::mvThemeColor(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvThemeColor::draw(ImDrawList* drawlist, float x, float y)
	{
		ImVec4 color = ImVec4((*_value)[0], (*_value)[1], (*_value)[2], (*_value)[3]);

		if (_libType == mvLibType::MV_IMGUI)
		{
			if (_targetColor == ImGuiCol_DockingPreview)
				GImGui->Style.Colors[_targetColor] = color;

			ImGui::PushStyleColor(_targetColor, color);
		}
		else if (_libType == mvLibType::MV_IMPLOT)
			ImPlot::PushStyleColor(_targetColor, color);
		else if (_libType == mvLibType::MV_IMNODES)
			imnodes::PushColorStyle((imnodes::ColorStyle)_targetColor, ImGui::ColorConvertFloat4ToU32(color));
	}

	void mvThemeColor::customAction(void* data)
	{
		if (_libType == mvLibType::MV_IMGUI)
			ImGui::PopStyleColor();
		else if (_libType == mvLibType::MV_IMPLOT)
			ImPlot::PopStyleColor();
		else if (_libType == mvLibType::MV_IMNODES)
			imnodes::PopColorStyle();
	}

	void mvThemeColor::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_targetColor = ToInt(item);
				break;

			case 1:
				setPyValue(item);
				break;

			default:
				break;
			}
		}
	}

	void mvThemeColor::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "category")) _libType = (mvLibType)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "value")) setPyValue(item);

		if (_libType == mvLibType::MV_IMGUI)
		{

			if (_targetColor >= ImGuiCol_COUNT || _targetColor < 0)
			{
				_state.ok = false;
				mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
				assert(false);
			}
		}

		else if (_libType == mvLibType::MV_IMPLOT)
		{
			if (_targetColor >= ImPlotCol_COUNT || _targetColor < 0)
			{
				_state.ok = false;
				mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
				assert(false);
			}
		}

		else if (_libType == mvLibType::MV_IMNODES)
		{
			if (_targetColor >= imnodes::ColorStyle_Count || _targetColor < 0)
			{
				_state.ok = false;
				mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
				assert(false);
			}
		}
	}

	void mvThemeColor::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		mvPyObject py_target = ToPyInt(_targetColor);
		mvPyObject py_cat = ToPyInt((int)_libType);

		PyDict_SetItemString(dict, "target", py_target);
		PyDict_SetItemString(dict, "category", py_cat);
	}

	PyObject* mvThemeColor::getPyValue()
	{
		// nasty hack
		int r = (int)(_value->data()[0] * 255.0f * 255.0f);
		int g = (int)(_value->data()[1] * 255.0f * 255.0f);
		int b = (int)(_value->data()[2] * 255.0f * 255.0f);
		int a = (int)(_value->data()[3] * 255.0f * 255.0f);

		auto color = mvColor(r, g, b, a);
		return ToPyColor(color);
	}

	void mvThemeColor::setPyValue(PyObject* value)
	{
		mvColor color = ToColor(value);
		std::array<float, 4> temp_array;
		temp_array[0] = color.r;
		temp_array[1] = color.g;
		temp_array[2] = color.b;
		temp_array[3] = color.a;
		if (_value)
			*_value = temp_array;
		else
			_value = std::make_shared<std::array<float, 4>>(temp_array);

		if (_parentPtr)
			_parentPtr->_triggerAlternativeAction = true;
	}

	void mvThemeColor::setDataSource(mvUUID dataSource)
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
		_value = *static_cast<std::shared_ptr<std::array<float, 4>>*>(item->getValue());
	}

	void mvThemeColor::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvThemeColor*>(item);
		if (_source != 0) _value = titem->_value;
		_targetColor = titem->_targetColor;
		_libType = titem->_libType;
	}
}
