#pragma once

#include <variant>

//-----------------------------------------------------------------------------
// mvAppItemDescription
//
//     - mvAppItemDescription holds the descripton of the particular item.
//     
//-----------------------------------------------------------------------------

// forward declarations
struct ImPlotTime;

namespace Marvel {

    enum class StorageValueTypes
    {
        None = 0,
        Int, Int4,
        Float, Float4, FloatVect, 
        Series,
        Bool,
        String,
        Time, Color
    };

    enum ItemDescriptionFlags
    {
        MV_ITEM_DESC_DEFAULT     = 0,
        MV_ITEM_DESC_ROOT        = 1 << 1,
        MV_ITEM_DESC_CONTAINER   = 1 << 2,
        MV_ITEM_DESC_AFTER       = 1 << 3,
    };

    using mvValueVariant = std::variant<
        std::shared_ptr<int>,
        std::shared_ptr<std::array<int, 4>>,
        std::shared_ptr<float >,
        std::shared_ptr<std::array<float, 4>>,
        std::shared_ptr<std::vector<float>>,
        std::shared_ptr<std::vector<std::vector<float>>>,
        std::shared_ptr<bool>,
        std::shared_ptr<std::string>,
        std::shared_ptr<tm>,
        std::shared_ptr<ImPlotTime>,
        void*>;
}