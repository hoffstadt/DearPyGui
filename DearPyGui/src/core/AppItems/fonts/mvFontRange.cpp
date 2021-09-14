#include "mvFontRange.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void mvFontRange::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		args.push_back({ mvPyDataType::Integer, "first_char" });
		args.push_back({ mvPyDataType::Integer, "last_char" });

		mvPythonParserSetup setup;
		setup.category = { "Fonts", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvFontRange::mvFontRange(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvFontRange::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvFontRange*>(item);
		_min = titem->_min;
		_max = titem->_max;
		_range = titem->_range;
	}

	void mvFontRange::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(mvApp::GetApp()->getParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				_min = ToInt(item);
				_range[0] = (ImWchar)_min;
				break;
			}

			case 1:
			{
				_max = ToInt(item);
				_range[1] = (ImWchar)_max;
				break;
			}

			default:
				break;
			}
		}
	}
}