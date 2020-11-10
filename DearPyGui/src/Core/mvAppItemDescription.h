#pragma once

//-----------------------------------------------------------------------------
// mvAppItemDescription
//
//     - mvAppItemDescription holds the descripton of the particular item.
//     
//-----------------------------------------------------------------------------

namespace Marvel {

    struct mvAppItemDescription
    {

        bool root              = false;
        bool container         = false;
        bool duplicatesAllowed = false;
        bool disableAllowed    = false;
        bool deleteAllowed     = true ;

    };
}