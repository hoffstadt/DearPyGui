#include "mvFontChars.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "textures/mvStaticTexture.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

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

	auto charVect = ToIntVect(PyTuple_GetItem(dict, 0));
	for (auto& item : charVect)
		_chars.push_back((ImWchar)item);

}