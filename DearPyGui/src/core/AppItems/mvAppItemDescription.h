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
        Int, Int2, Int3, Int4,
        Float, Float2, Float3, Float4, FloatVect,
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
        std::shared_ptr<std::array<int, 2>>,
        std::shared_ptr<std::array<int, 3>>,
        std::shared_ptr<std::array<int, 4>>,
        std::shared_ptr<float >,
        std::shared_ptr<std::array<float, 2>>,
        std::shared_ptr<std::array<float, 3>>,
        std::shared_ptr<std::array<float, 4>>,
        std::shared_ptr<std::vector<float>>,
        std::shared_ptr<bool>,
        std::shared_ptr<std::string>,
        std::shared_ptr<tm>,
        std::shared_ptr<ImPlotTime>,
        void*>;
}