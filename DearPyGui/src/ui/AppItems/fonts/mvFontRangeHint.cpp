#include "mvFontRangeHint.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

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

	_hint = ToInt(PyTuple_GetItem(dict, 0));
}