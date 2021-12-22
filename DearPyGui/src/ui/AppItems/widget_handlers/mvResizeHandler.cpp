#include "mvResizeHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"
#include "containers/mvWindowAppItem.h"

namespace Marvel {

    mvResizeHandler::mvResizeHandler(mvUUID uuid)
        :
        mvAppItem(uuid)
    {

    }

    void mvResizeHandler::customAction(void* data)
    {
        mvAppItemState* state = static_cast<mvAppItemState*>(data);
        if (state->mvRectSizeResized)
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
