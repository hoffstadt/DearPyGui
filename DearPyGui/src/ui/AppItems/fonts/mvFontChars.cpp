#include "mvFontChars.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

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
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
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