#pragma once

//-----------------------------------------------------------------------------
// mvAppItemDescription
//
//     - mvAppItemDescription holds the descripton of the particular item.
//     
//-----------------------------------------------------------------------------

namespace Marvel {

    enum class StorageValueTypes
    {
        None = 0,
        Int, Int2, Int3, Int4,
        Float, Float2, Float3, Float4, FloatVect,
        Bool,
        String,
        Time, Color
    };

    struct mvAppItemDescription
    {

        bool root              = false;
        bool container         = false;
        bool duplicatesAllowed = false;
        bool disableAllowed    = false;
        bool deleteAllowed     = true ;
        bool ignoreSizeUpdate  = false;
        StorageValueTypes valueType = StorageValueTypes::None;

    };
}