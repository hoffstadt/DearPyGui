#include "mvFontRangeHint.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void mvFontRangeHint::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		args.push_back({ mvPyDataType::Integer, "hint" });

		mvPythonParserSetup setup;
		setup.about = "Adds a range of font characters (mvFontRangeHint_ constants).";
		setup.category = { "Fonts", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvFontRangeHint::mvFontRangeHint(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvFontRangeHint::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvFontRangeHint*>(item);
		_hint = titem->_hint;
	}

	void mvFontRangeHint::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
			{
				_hint = ToInt(item);
				break;
			}


			default:
				break;
			}
		}
	}
}