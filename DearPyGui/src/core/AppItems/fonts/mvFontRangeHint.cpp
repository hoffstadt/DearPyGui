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

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Fonts", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		parser.addArg<mvPyDataType::Integer>("hint");

		parser.finalize();

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
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
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