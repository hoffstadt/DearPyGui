#include "mvFocusHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

    mvFocusHandler::mvFocusHandler(mvUUID uuid)
        :
        mvAppItem(uuid)
    {

    }

    void mvFocusHandler::customAction(void* data)
    {

        mvAppItemState* state = static_cast<mvAppItemState*>(data);
        if (state->focused)
        {
            mvSubmitCallback([=]()
                {
                    if(config.alias.empty())
                        mvRunCallback(getCallback(false), uuid, ToPyUUID(state->parent), config.user_data);
                    else
                        mvRunCallback(getCallback(false), config.alias, ToPyUUID(state->parent), config.user_data);
                });
        }
    }

}
