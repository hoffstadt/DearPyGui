#include "mvFontRange.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

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

	_min = ToInt(PyTuple_GetItem(dict, 0));
	_range[0] = (ImWchar)_min;
	_max = ToInt(PyTuple_GetItem(dict, 1));
	_range[1] = (ImWchar)_max;
}