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

    struct mvAppItemDescription
    {

        bool root              = false;
        bool container         = false;
        bool duplicatesAllowed = false;
        bool disableAllowed    = false;
        bool deleteAllowed     = true ;
        bool ignoreSizeUpdate  = false;
        bool addAfterRequired  = false;
        StorageValueTypes valueType = StorageValueTypes::None;

    };
}