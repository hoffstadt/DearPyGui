#pragma once

//-----------------------------------------------------------------------------
// mvAppItemDescription
//
//     - mvAppItemDescription holds the descripton of the perticular item.
//     
//-----------------------------------------------------------------------------

namespace Marvel {

    struct mvAppItemDescription
    {

    public:

        // getters
        [[nodiscard]] bool      isARoot() const                 { return root; }
        [[nodiscard]] bool      isContainer() const             { return container; }
        [[nodiscard]] bool      canBeDuplicated() const         { return duplicatesAllowed; }
        [[nodiscard]] bool      canBeDeleted() const            { return deleteAllowed; }
        [[nodiscard]] bool      canBeDisabled() const           { return disableAllowed; }

        // descriptions
        bool    root = false;
        bool    container = false;
        bool    duplicatesAllowed = false;
        bool    disableAllowed = false;
        bool    deleteAllowed = true;

    };
}