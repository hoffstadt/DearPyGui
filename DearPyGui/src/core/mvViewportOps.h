#pragma once

//-----------------------------------------------------------------------------
// mvWindow
//
//     - This abstract class is just a light wrapper for a platform specific
//       window. It's quite lacking so may need to be extended later.
//
//     - This class may need to be renamed to remove confusion between this
//       window and the mvWindowAppItem class (maybe to viewport?)
//     
//-----------------------------------------------------------------------------

#include "mvContext.h"
#include "mvEvents.h"
#include "cpp.hint"
#include <imgui.h>

namespace Marvel {

	void InsertParser_mvViewport(std::map<std::string, mvPythonParser>* parsers);

	MV_CREATE_FREE_COMMAND(show_viewport);
	MV_CREATE_FREE_COMMAND(create_viewport);
	MV_CREATE_FREE_COMMAND(configure_viewport);
	MV_CREATE_FREE_COMMAND(get_viewport_configuration);
	MV_CREATE_FREE_COMMAND(is_viewport_ok);

	// viewport operations
	MV_CREATE_FREE_COMMAND(maximize_viewport);
	MV_CREATE_FREE_COMMAND(minimize_viewport);

	MV_START_FREE_COMMANDS(mvViewportCommands)
		MV_ADD_COMMAND(show_viewport)
		MV_ADD_COMMAND(create_viewport)
		MV_ADD_COMMAND(configure_viewport)
		MV_ADD_COMMAND(maximize_viewport)
		MV_ADD_COMMAND(minimize_viewport)
		MV_ADD_COMMAND(get_viewport_configuration)
		MV_ADD_COMMAND(is_viewport_ok)
	MV_END_COMMANDS

}
