#include "mvCharRemap.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void mvCharRemap::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		args.push_back({ mvPyDataType::Integer, "source"});
		args.push_back({ mvPyDataType::Integer, "target"});

		mvPythonParserSetup setup;
		setup.category = { "Fonts", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvCharRemap::mvCharRemap(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvCharRemap::handleSpecificRequiredArgs(PyObject* dict)
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
				_source = ToInt(item);
				break;
			}

			case 1:
			{
				_target = ToInt(item);
				break;
			}

			default:
				break;
			}
		}
	}
}