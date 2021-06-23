#pragma once

#include <string>

namespace Marvel
{
	enum class mvErrorCode
	{
		mvNone                = 1000,
		mvTextureNotFound     = 1001,
		mvIncompatibleType    = 1002,
		mvIncompatibleParent  = 1003,
		mvIncompatibleChild   = 1004,
		mvItemNotFound        = 1005,
		mvSourceNotFound      = 1006,
		mvSourceNotCompatible = 1007,
		mvWrongType           = 1008,
		mvContainerStackEmpty = 1009,
		mvStagingModeOff      = 1010,
		mvParentNotDeduced    = 1011,
	};

	// forward declaration
	class mvAppItem;

	void mvThrowPythonError(mvErrorCode code, const std::string& message);
	void mvThrowPythonError(mvErrorCode code, const std::string& command, const std::string& message, mvAppItem* item);
}
