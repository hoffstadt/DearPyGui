#include "mvFontChars.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void mvFontChars::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		args.push_back({ mvPyDataType::IntList, "chars" });

		mvPythonParserSetup setup;
		setup.category = { "Fonts", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvFontChars::mvFontChars(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvFontChars::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvFontChars*>(item);
		_chars = titem->_chars;
	}

	void mvFontChars::handleSpecificRequiredArgs(PyObject* dict)
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
				auto charVect = ToIntVect(item);
				for (auto& item : charVect)
					_chars.push_back((ImWchar)item);
				break;
			}

			default:
				break;
			}
		}
	}
}