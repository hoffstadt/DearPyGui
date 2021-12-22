#include "mvFontRangeHint.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

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
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
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