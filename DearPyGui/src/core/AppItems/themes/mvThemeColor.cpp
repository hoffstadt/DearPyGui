#include "mvThemeColor.h"
#include "mvApp.h"
#include <array>
#include <implot.h>
#include <imnodes.h>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvThemeColor::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Themes" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		parser.addArg<mvPyDataType::Long>("target", mvArgType::POSITIONAL_ARG, "0");
		parser.addArg<mvPyDataType::IntList>("value", mvArgType::POSITIONAL_ARG, "(0, 0, 0, 255)");
		parser.addArg<mvPyDataType::Integer>("category", mvArgType::KEYWORD_ARG, "0", "Options include mvThemeCat_Core, mvThemeCat_Plots, mvThemeCat_Nodes.");

		parser.finalize();

		parsers->insert({ s_command, parser });

	}

	mvThemeColor::mvThemeColor(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvThemeColor::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTheme) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: mvTheme", this);

		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvThemeColor::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_libType == mvLibType::MV_IMGUI)
			ImGui::PushStyleColor(_targetColor, _color.toVec4());
		else if (_libType == mvLibType::MV_IMPLOT)
			ImPlot::PushStyleColor(_targetColor,_color.toVec4());
		else if (_libType == mvLibType::MV_IMNODES)
			imnodes::PushColorStyle((imnodes::ColorStyle)_targetColor, mvColor::ConvertToUnsignedInt(_color));
	}

	void mvThemeColor::customAction()
	{
		if (_libType == mvLibType::MV_IMGUI)
			ImGui::PopStyleColor();
		else if (_libType == mvLibType::MV_IMPLOT)
			ImPlot::PopStyleColor();
		else if (_libType == mvLibType::MV_IMNODES)
			imnodes::PopColorStyle();
	}

	void mvThemeColor::alternativeCustomAction()
	{
		if (_libType == mvLibType::MV_IMGUI)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[_targetColor] = _color.toVec4();
		}
		else if (_libType == mvLibType::MV_IMPLOT)
		{
			ImPlotStyle& plotstyle = ImPlot::GetStyle();
			plotstyle.Colors[_targetColor] = _color.toVec4();
		}
		else if (_libType == mvLibType::MV_IMNODES)
		{
			imnodes::GetStyle().colors[_targetColor] = ImGui::ColorConvertFloat4ToU32(_color);
		}
	}

	void mvThemeColor::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyPositionalArguments(dict))
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
				_color = ToColor(item);
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
		if (PyObject* item = PyDict_GetItemString(dict, "value")) _color = ToColor(item);

		if (_libType == mvLibType::MV_IMGUI)
		{

			if (_targetColor >= ImGuiCol_COUNT || _targetColor < 0)
			{
				_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
				assert(false);
			}
		}

		else if (_libType == mvLibType::MV_IMPLOT)
		{
			if (_targetColor >= ImPlotCol_COUNT || _targetColor < 0)
			{
				_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
				assert(false);
			}
		}

		else if (_libType == mvLibType::MV_IMNODES)
		{
			if (_targetColor >= imnodes::ColorStyle_Count || _targetColor < 0)
			{
				_state.setOk(false);
				mvThrowPythonError(mvErrorCode::mvNone, "Style target out of range.");
				MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
				assert(false);
			}
		}
	}

}
