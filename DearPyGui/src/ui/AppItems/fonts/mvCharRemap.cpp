#include "mvCharRemap.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	mvCharRemap::mvCharRemap(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvCharRemap::handleSpecificRequiredArgs(PyObject* dict)
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