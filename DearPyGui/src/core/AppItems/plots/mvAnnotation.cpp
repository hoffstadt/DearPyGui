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
		: mvDouble4PtrBase(uuid)
	{
	}

	bool mvAnnotation::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot, staging container", this);

		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
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