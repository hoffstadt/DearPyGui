#include "mvDeactivatedAfterEditHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

    mvDeactivatedAfterEditHandler::mvDeactivatedAfterEditHandler(mvUUID uuid)
        :
        mvAppItem(uuid)
    {

    }

    void mvDeactivatedAfterEditHandler::customAction(void* data)
    {

        mvAppItemState* state = static_cast<mvAppItemState*>(data);
        if (state->deactivatedAfterEdit)
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
