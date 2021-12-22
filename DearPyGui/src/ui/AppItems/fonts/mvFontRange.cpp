#include "mvFontRange.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

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
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
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