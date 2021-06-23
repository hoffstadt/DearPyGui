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

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Fonts", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT)
		);

		parser.addArg<mvPyDataType::Integer>("first_char");
		parser.addArg<mvPyDataType::Integer>("last_char");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvFontRange::mvFontRange(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	bool mvFontRange::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvFont) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: font", this);
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvFontRange::handleSpecificRequiredArgs(PyObject* dict)
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
				m_min = ToInt(item);
				m_range[0] = (ImWchar)m_min;
				break;
			}

			case 1:
			{
				m_max = ToInt(item);
				m_range[1] = (ImWchar)m_max;
				break;
			}


			default:
				break;
			}
		}
	}
}