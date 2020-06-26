#include "mvInterfaces.h"
#include "mvModuleInitializer.h"

namespace Marvel {

	int mvModuleInitializer::initializeCoreModules()
	{
		initModule<0>("sbApp", CreatePythonInterface, BuildAppInterface);
		initModule<1>("sbInput", CreateInputInterface, BuildInputsInterface);
		initModule<2>("sbLog", CreateLoggerInterface, BuildLoggingInterface);
		initModule<3>("sbPlot", CreatePlotInterface, BuildPlottingInterface);
		initModule<4>("sbDraw", CreateDrawingInterface, BuildDrawingInterface);
		initModule<5>("sbWidgets", CreateWidgetAddingInterface, BuildWidgetsInterface);
		initModule<6>("sbConstants", CreateConstantsInterface);

		return 7;
	}

}