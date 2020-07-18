#include "mvInterfaceRegistry.h"
#include "mvModuleInitializer.h"

namespace Marvel {

	int mvModuleInitializer::initializeCoreModules()
	{
		initModule<0>("marvel", CreateMarvelInterface, BuildMarvelInterface);
		initModule<1>("marvel_constants", CreateConstantsInterface);

		return 2;
	}

}