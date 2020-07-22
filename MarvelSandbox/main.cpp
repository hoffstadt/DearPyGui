#include "MarvelApplication.h"

using namespace Marvel;

int main(int argc, char* argv[])
{

	Application app("MarvelSandbox", argc, argv);

	app.parseCommandLine();

#ifdef MV_RELEASE
	app.hideConsole();
#else
	app.showConsole();
#endif

	app.handlePaths();

	app.initializePython();

	app.logInformation();

	app.importModule();

	app.run();

	return 0;
}