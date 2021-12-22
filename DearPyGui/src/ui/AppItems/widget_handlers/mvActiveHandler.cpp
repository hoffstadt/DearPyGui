#include "mvActiveHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

    mvActiveHandler::mvActiveHandler(mvUUID uuid)
        :
        mvAppItem(uuid)
    {

    }

    void mvActiveHandler::customAction(void* data)
    {

        mvAppItemState* state = static_cast<mvAppItemState*>(data);
        if (state->active)
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
