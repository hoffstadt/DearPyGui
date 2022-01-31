#include "mvCharRemap.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

mvCharRemap::mvCharRemap(mvUUID uuid)
	:
	mvAppItem(uuid)
{

}

void mvCharRemap::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_source = ToInt(PyTuple_GetItem(dict, 0));
	_target = ToInt(PyTuple_GetItem(dict, 1));
}